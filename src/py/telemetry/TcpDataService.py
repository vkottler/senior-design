
"""
Fault-Tolerant Quadcopter - raw data handling
"""

# internal
from .TcpTelemetryService import TcpTelemetryService

def TcpDataHandler(data, wfile):
    """ Handle incoming data. """

    print("data: " + str(data)) 

class TcpDataService(TcpTelemetryService):
    """ Data service wrapper. """

    def __init__(self, port):
        """ Initialize this tcp service. """

        super().__init__(port, "data", TcpDataHandler)
