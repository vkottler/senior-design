
"""
Fault-Tolerant Quadcopter - raw command handling
"""

# internal
from .TcpTelemetryService import TcpTelemetryService

def TcpCommandHandler(data, wfile):
    """ Handle incoming commands. """

    print("command: " + str(data)) 

class TcpCommandService(TcpTelemetryService):
    """ Command service wrapper. """

    def __init__(self, port):
        """ Initialize this tcp service. """

        super().__init__(port, "command", TcpCommandHandler)
