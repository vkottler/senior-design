
"""
Fault-Tolerant Quadcopter - raw data handling
"""

# built-in
import logging
from socketserver import StreamRequestHandler

# internal
from .TcpTelemetryService import TcpTelemetryService

class TcpDataHandler(StreamRequestHandler):
    """ Data handler for tcp-based data services. """

    log = logging.getLogger(__name__)

    def handle(self):
        """ Handle incoming data. """

        TcpDataService.log_client_action(self.client_address, "connected")

        while not self.server.telemetry_shutdown_request:
            # TODO: find a way to not block here
            data = self.rfile.readline().strip().decode("utf-8")

            if not data or data == "":
                TcpDataService.log_client_action(self.client_address,
                                                 "disconnected")
                return

            print("data: " + data) 

class TcpDataService(TcpTelemetryService):
    """ Data service wrapper. """

    @staticmethod
    def log_client_action(client, action):
        """ Convenient logging abstraction. """

        TcpTelemetryService.log_client_action(TcpDataHandler.log,
                                              client, action)

    def __init__(self, port):
        """ Initialize this tcp service. """

        super().__init__(port, "data", TcpDataHandler)
