
"""
Fault-Tolerant Quadcopter - generic telemetry service wrapper
"""

# built-in
import logging

# internal
from .Daemon import Daemon

class TelemetryService(Daemon):
    """ A wrapper for Daemons that leverage serversocket semantics. """

    log = logging.getLogger(__name__)

    def start_generator(self, server, message="null"):
        """
        Generate a 'start' function for a Daemon with serversocket semantics.
        """

        def start_fn():
            """ Start a server and report it to the logger. """

            TelemetryService.log.info("'%s' service started (%s).",
                                      self.name, message)
            server.serve_forever()

        return start_fn

    def stop_generator(self, server, message="None"):
        """
        Generate a 'stop' function for a Daemon with serversocket semantics.
        """

        def stop_fn():
            """ Stop a server and report it to the logger. """

            TelemetryService.log.info("'%s' service stopped (%s).",
                                      self.name, message)
            server.telemetry_shutdown_request = True
            server.shutdown()
            server.server_close()

        return stop_fn

    def __init__(self, name, server, metadata="None"):
        """ Create the start and stop functions and instantiate the Daemon. """

        self.server = server
        self.server.telemetry_shutdown_request = False
        run_fn = self.start_generator(self.server, metadata)
        stop_fn = self.stop_generator(self.server, metadata)
        super().__init__(name, run_fn, stop_fn)
