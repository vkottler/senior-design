
"""
Fault-Tolerant Quadcopter - simulation execution
"""

# built-in
import logging
import sys

# internal
from .classes.Quadcopter import Quadcopter
from .classes.SimulationEngine import SimulationEngine
from .classes.telemetry.Server import TelemetryServer

def simulate(parsed):
    """ """

    log = logging.getLogger(__name__)

    # initialize telemetry server
    server = TelemetryServer(0, "Simulation Server")
    server.start()

    # initialize quadcopter
    quadcopter = Quadcopter(server.port())

    # initialize the simulation environment
    engine = SimulationEngine(quadcopter, parsed.step_duration, parsed.real_time)

    # run the simulation to completion
    time_remaining = parsed.duration * 1000
    total_steps = 0
    while time_remaining:
        engine.step()
        time_remaining -= parsed.step_duration
        if total_steps % 10 == 0:
            sys.stdout.write("\r{0}s".format(time_remaining / 1000))
        total_steps += 1
    sys.stdout.write("\r")

    log.info(engine.metrics())

    # stop telemetry
    quadcopter.telemetry.stop()
    server.stop()

    return 0
