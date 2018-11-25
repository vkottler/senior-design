
"""
Fault-Tolerant Quadcopter - raw command handling
"""

# built-in
import logging
from socketserver import StreamRequestHandler

# internal
from .TcpTelemetryService import TcpTelemetryService

class TcpCommandHandler(StreamRequestHandler):
    """ Command handler for tcp-based command services. """

    log = logging.getLogger(__name__)

    def handle(self):
        """ Handle incoming commands. """

        TcpCommandService.log_client_action(self.client_address, "connected")

        while not self.server.telemetry_shutdown_request:
            # TODO: find a way to not block here
            data = self.rfile.readline().strip().decode("utf-8")

            if not data or data == "":
                TcpCommandService.log_client_action(self.client_address,
                                                    "disconnected")
                return

            print("command: " + data) 

class TcpCommandService(TcpTelemetryService):
    """ Command service wrapper. """

    @staticmethod
    def log_client_action(client, action):
        """ Convenient logging abstraction. """

        TcpTelemetryService.log_client_action(TcpCommandHandler.log,
                                              client, action)

    def __init__(self, port):
        """ Initialize this tcp service. """

        super().__init__(port, "command", TcpCommandHandler)
