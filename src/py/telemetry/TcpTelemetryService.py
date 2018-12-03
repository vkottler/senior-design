
"""
Fault-Tolerant Quadcopter - generic tcp service wrapper
"""

# built-in
import fcntl
import os
import logging
from socketserver import ThreadingTCPServer, StreamRequestHandler
from time import sleep

# internal
from .TelemetryService import TelemetryService

class TcpTelemetryHandler(StreamRequestHandler):
    """
    Handle the stream and connection lifecycle, call a provided handler
    when new data is available.
    """

    log = logging.getLogger(__name__)

    def log_action(self, action):
        """ Log a connection event for this connection. """

        TcpTelemetryService.log_client_action(self.server.service_name,
                                              self.client_address, action)

    def handle(self):
        """ Handle incoming data. """

        self.log_action("connected")

        if not TcpTelemetryService.set_nonblocking(self.rfile):
            TcpTelemetryHandler.log.error("couldn't set reads to non-blocking")
            return

        stream_name = "{0}-{1}:{2}-input".format(self.server.service_name,
                                                 self.client_address[0],
                                                 self.client_address[1])
        if self.server.input_stream is not None:
            self.server.input_stream.add_output(stream_name, self.wfile)

        # continue awaiting 
        while True:
            data = None
            while data is None and not self.server.telemetry_shutdown_request:
                try:
                    TcpTelemetryService.poll_sleep()
                    data = self.rfile.read()
                except BlockingIOError:
                    pass

            # we want to exit
            if self.server.telemetry_shutdown_request:
                self.log_action("closed by us")
                break

            # client closed the connection if we read 0 bytes
            if not len(data):
                self.log_action("disconnected")
                break

            # operate on the data
            self.server.service_handler(data, self.wfile, self.server.stream)

        if self.server.input_stream is not None:
            self.server.input_stream.remove_output(stream_name)

class TcpTelemetryService(TelemetryService):
    """ Wrapper for servers leveraging ThreadingTCPServer. """

    poll_sleep_ms = 0.05

    @staticmethod
    def log_client_action(service_name, client, action):
        """ Convenient logging abstraction. """

        TcpTelemetryHandler.log.info("%s client %s: %s:%d", service_name,
                                     action, client[0], client[1])

    @staticmethod
    def set_nonblocking(file_obj):
        """ Make read operations on a file object not block. """

        fd = file_obj.fileno()
        flag = fcntl.fcntl(fd, fcntl.F_GETFL)
        fcntl.fcntl(fd, fcntl.F_SETFL, flag | os.O_NONBLOCK)
        flag = fcntl.fcntl(fd, fcntl.F_GETFL)
        if not flag & os.O_NONBLOCK:
            return False
        return True

    @staticmethod
    def poll_sleep():
        """ Provide a mechanism to sleep some amount when polling. """

        sleep(TcpTelemetryService.poll_sleep_ms)

    def __init__(self, port, service_name, service_handler, stream,
                 input_stream):
        """ Initialize the tcp telemetry-service daemon. """

        self.address = ("", port)
        ThreadingTCPServer.allow_reuse_address = True
        self.server = ThreadingTCPServer(self.address, TcpTelemetryHandler)
        self.server.service_handler = service_handler
        self.server.service_name = service_name
        self.server.stream = stream
        self.server.input_stream = input_stream
        super().__init__(service_name, self.server, str(port))
