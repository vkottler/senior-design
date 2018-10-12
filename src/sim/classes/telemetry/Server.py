
"""
Fault-Tolerant Quadcopter - telemetry server
"""

# built-in
import logging
import queue
import socketserver
import threading

class DefaultTelemetryHandler(socketserver.StreamRequestHandler):
    """ """

    def handle(self):
        """ """

        while True:

            data = self.rfile.readline().strip()

            if not data or data == "":
                TelemetryServer.log.info("A client connection was closed.")
                return

            # convert JSON back to Channel
            # TODO
            print(data)

            # push to a concurrent data structure owned by self.server
            #self.server.data.put()

class TelemetryServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    """ """

    # kill active connections on exit + much faster rebinding
    daemon_threads = True
    allow_reuse_address = True

    log = logging.getLogger(__name__)

    def __init__(self, port=10018, name="Generic Server"):
        """ """

        self.name = name
        super().__init__(("127.0.0.1", port), DefaultTelemetryHandler)
        self.data = queue.Queue()
        self.thread = threading.Thread(target=self.serve_forever)

    def start(self):
        """ """

        self.thread.start()
        TelemetryServer.log.info("'%s' was started.", self.name)

    def stop(self):
        """ """

        self.shutdown()
        self.thread.join()
        self.server_close()
        TelemetryServer.log.info("'%s' was stopped.", self.name)
