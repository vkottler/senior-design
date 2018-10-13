
"""
Fault-Tolerant Quadcopter - telemetry handling actions
"""

# built-in
import logging
import socketserver

# internal
from .Channel import Channel

class DefaultTelemetryHandler(socketserver.StreamRequestHandler):
    """
    A serversocket connection handler implementation for the Telemetry Server.
    """

    log = logging.getLogger(__name__)

    def handle(self):
        """
        This method runs for all active client connections, data is read in
        line-by-line.
        """

        while True:

            data = self.rfile.readline().strip().decode("utf-8")

            if not data or data == "":
                DefaultTelemetryHandler.log.info("A client connection was closed.")
                return

            channel = Channel.from_json(data, None)
            DefaultTelemetryHandler.log.info("%s: %s (%s)", channel.dict["name"],
                                             channel.dict["value"], channel.dict["type"])

            # push to a concurrent data structure owned by self.server
            #self.server.data.put()
