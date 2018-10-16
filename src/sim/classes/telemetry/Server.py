
"""
Fault-Tolerant Quadcopter - telemetry server
"""

# built-in
import logging
import queue
import socketserver
import threading

# internal
from .DataHandling import DefaultTelemetryHandler
from .DataBuffer import TelemetryBuffer

class TelemetryServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    """
    A TCP-message server that accepts incoming telemetry
    (published Channel data) and 
    """

    # kill active connections on exit + faster rebinding
    daemon_threads = True
    allow_reuse_address = True

    log = logging.getLogger(__name__)

    def __init__(self, port=10018, name="Generic Server",
                 handle_class=DefaultTelemetryHandler):
        """ Initialize the Telemetry Server. """

        self.name = name
        super().__init__(("127.0.0.1", port), handle_class)
        self.data = queue.Queue()
        self.thread = threading.Thread(target=self.serve_forever)
        self.started = False
        self.stopped = False
        self.buffer = TelemetryBuffer()

    def port(self):
        """ Return the port that this server is attached to. """

        return self.server_address[1]

    def start(self):
        """ Start serving Telemetry Clients. """

        # check to see if starting this Server makes sense
        if self.started:
            TelemetryServer.log.error("'%s' can't be started.", self.name)
            return False

        self.thread.start()
        TelemetryServer.log.info("'%s' was started on port %d.", self.name, self.port())
        self.started = True
        return True

    def stop(self):
        """ Stop listening for new (and servicing existing) Telemetry Clients. """

        # check to see if stopping this Server makes sense
        if not self.started or self.stopped:
            TelemetryServer.log.error("'%s' can't be stopped.", self.name)
            return False

        self.stopped = True
        self.shutdown()
        self.thread.join()
        self.server_close()
        TelemetryServer.log.info("'%s' was stopped.", self.name)
        return True
