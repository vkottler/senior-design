
"""
Fault-Tolerant Quadcopter - raw data handling
"""

# internal
from .TcpTelemetryService import TcpTelemetryService

def TcpDataHandler(data, wfile, stream):
    """ Handle incoming data. """

    # TODO: decode data as a packet, write an ack to wfile?
    # write decoded data to subscribing streams
    stream.write(data)

class TcpDataService(TcpTelemetryService):
    """ Data service wrapper. """

    def __init__(self, port, stream, input_stream=None):
        """ Initialize this tcp service. """

        super().__init__(port, "data", TcpDataHandler, stream, input_stream)
