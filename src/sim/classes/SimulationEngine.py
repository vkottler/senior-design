
"""
Fault-Tolerant Quadcopter - coordinates simulation execution
"""

# built-in
import time

# internal
from .Quadcopter import Quadcopter

class SimulationEngine(object):
    """ """

    def __init__(self, step_duration=1, real_time=False):
        """ """

        self.step_duration = step_duration
        self.real_time = real_time
        self.computation_times = []

        # TODO: do we want this here?
        self.quadcopter = Quadcopter()

    def step(self):
        """ """

        start = time.time()

        # do math
        result = 9999999 * 999999999
        time.sleep(0.0001)

        end = time.time()

        # store values in microseconds
        self.computation_times.append((end - start) * 1000)

        # wait the full step duration when running in real time
        if self.real_time:
            current = time.time()
            while current - start < (self.step_duration / float(1000)):
                current = time.time()

        return result

    def metrics(self):
        """ """

        metrics = {}
        metrics["iterations"] = len(self.computation_times)
        metrics["longest"] = max(self.computation_times)
        metrics["shortest"] = min(self.computation_times)
        metrics["average"] = sum(self.computation_times) / float(len(self.computation_times))

        return metrics
