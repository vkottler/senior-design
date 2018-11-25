
"""
Fault-Tolerant Quadcopter - generic tcp service wrapper
"""

# built-in
from socketserver import ThreadingTCPServer

# internal
from .TelemetryService import TelemetryService

class TcpTelemetryService(TelemetryService):
    """ Wrapper for servers leveraging ThreadingTCPServer. """

    @staticmethod
    def log_client_action(log, client, action):
        """ Convenient logging abstraction. """

        log.info("Client %s: %s:%d", action, client[0], client[1])

    def __init__(self, port, service_name, service_handler_class):
        """ Initialize the tcp telemetry-service daemon. """

        self.address = ("", port)
        ThreadingTCPServer.allow_reuse_address = True
        self.server = ThreadingTCPServer(self.address, service_handler_class)
        super().__init__(service_name, self.server, str(port))
