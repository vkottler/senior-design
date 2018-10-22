
"""
Fault-Tolerant Quadcopter - telemetry web services
"""

# built-in
from time import sleep

# internal
from .classes.telemetry.DataService import TelemetryService
from .classes.telemetry.Server import TelemetryServer

def serve(parsed):
    """ Serve the telemetry backend and web API. """

    # start telemetry server
    telemetry = TelemetryServer(parsed.port, "Telemetry Proxy")
    telemetry.start()

    # start web API
    service = TelemetryService(telemetry.buffer)
    service.start()

    while True:
        try:
            sleep(1000)
        except KeyboardInterrupt:
            telemetry.stop()
            service.stop()
            break
    return 0
