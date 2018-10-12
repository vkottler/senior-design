
"""
Fault-Tolerant Quadcopter - telemetry server
"""

# built-in
import queue
import socketserver
import threading

class DefaultTelemetryHandler(socketserver.StreamRequestHandler):
    """ """

    def handle(self):
        """ """

        while True:

            self.data = self.rfile.readline().strip()

            if data == "":
                print("A client connection was closed.")
                break

            # convert JSON back to Channel
            # TODO
            print(self.data)

            # push to a concurrent data structure owned by self.server
            #self.server.data.put()

class TelemetryServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    """ """

    # kill active connections on exit + much faster rebinding
    daemon_threads = True
    allow_reuse_address = True

    def __init__(self, port=10018):
        """ """

        super().__init__(("127.0.0.1", port), DefaultTelemetryHandler)
        self.data = queue.Queue()
        self.thread = threading.Thread(target=self.serve_forever)

    def start(self):
        """ """

        self.thread.start()
        # TODO, log?

    def stop(self):
        """ """

        self.shutdown()
        self.thread.join()
