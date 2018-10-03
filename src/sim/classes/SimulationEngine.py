
"""
Fault-Tolerant Quadcopter - coordinates simulation execution
"""

# built-in
import time

class SimulationEngine(object):
    """ """

    def __init__(self, step_duration=1, real_time=False):
        """ """

        self.step_duration = step_duration
        self.real_time = real_time
        self.computation_times = []

    def step(self):
        """ """

        start = time.time()

        # do math
        result = 9999999 * 999999999

        end = time.time()

        self.computation_times.append(end - start)

        # wait the full step duration when running in real time
        if self.real_time:
            current = time.time()
            while current - start < (self.step_duration / 1000):
                current = time.time()

        return result

    def metrics(self):
        """ """

        metrics = {}
        metrics["iterations"] = len(self.computation_times)
        metrics["longest_duration"] = 0
        metrics["shortest_duration"] = 0
        metrics["average_duration"] = 0

        return metrics
