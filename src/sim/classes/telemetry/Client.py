
"""
Fault-Tolerant Quadcopter - telemetry client
"""

# built-in
import logging
import queue
import socket
import threading

class TelemetryClient(object):
    """
    A TCP-messaging based Telemetry Client that publishes data to a 
    telemetry server.
    """

    log = logging.getLogger(__name__)

    def __init__(self, port=10018, name="Generic Client"):
        """ Initialize the Telemetry Client. """

        self.port = port
        self.name = name
        self.sock = socket.socket()
        self.data = queue.Queue()
        self.thread = threading.Thread(target=self.stream_telemetry)
        self.started = False
        self.stopped = False

    def stream_telemetry(self):
        """
        Write any requested Channel objects as JSON to the Telemetry Server
        as they appear in the queue.
        """

        while not self.stopped:
            try:
                channel_obj = self.data.get_nowait()
                self.sock.sendall(bytearray(channel_obj.to_json() + "\n", encoding="utf-8"))
                self.data.task_done()
            except queue.Empty:
                pass

    def start(self):
        """
        Attempt to connect to a Telemetry Server and run the telemetry
        service in another thread.
        """

        self.sock.connect(("127.0.0.1", self.port))
        self.thread.start()
        self.started = True
        TelemetryClient.log.info("'%s' was started.", self.name)

    def stop(self):
        """
        Disconnect from the Telemetry Server and stop the telemetry service.
        """

        self.stopped = True
        self.thread.join()
        self.sock.close()
        TelemetryClient.log.info("'%s' was stopped.", self.name)
