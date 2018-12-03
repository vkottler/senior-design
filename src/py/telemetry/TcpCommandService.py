
"""
Fault-Tolerant Quadcopter - raw command handling
"""

# internal
from .TcpTelemetryService import TcpTelemetryService

def TcpCommandHandler(data, wfile, stream):
    """ Handle incoming commands. """

    print("command data dropped: "+ data.encode("utf-8"))
    #stream.write(data)

class TcpCommandService(TcpTelemetryService):
    """ Command service wrapper. """

    def __init__(self, port, stream, input_stream=None):
        """ Initialize this tcp service. """

        super().__init__(port, "command", TcpCommandHandler, stream,
                         input_stream)
