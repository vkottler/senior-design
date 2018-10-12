
"""
Fault-Tolerant Quadcopter - telemetry client
"""

# built-in
import queue
import socket
import threading

class TelemetryClient(object):
    """ """

    def __init__(self, port=10018):
        """ """

        self.port = port
        self.sock = socket.socket()
        self.data = queue.Queue()
        self.thread = threading.Thread(self.stream_telemetry)

    def stream_telemetry(self):
        """ """

        while True:
            channel_data = self.data.get()
            # TODO, figure out JSON conversion

    def start(self):
        """ """

        self.sock.create_connection(("127.0.0.1", self.port))
        self.thread.start()
        # TODO, log?

    def stop(self):
        """ """

        self.sock.close()
        self.thread.join()
