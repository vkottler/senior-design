
"""
Fault-Tolerant Quadcopter - raw command handling
"""

# internal
from .TcpTelemetryService import TcpTelemetryService

def TcpCommandHandler(data, wfile, stream):
    """ Handle incoming commands. """

    stream.write(data)

class TcpCommandService(TcpTelemetryService):
    """ Command service wrapper. """

    def __init__(self, port, stream=None):
        """ Initialize this tcp service. """

        super().__init__(port, "command", TcpCommandHandler, stream)
