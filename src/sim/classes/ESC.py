
"""
Fault-Tolerant Quadcopter - simulated electronic speed-controller
"""

class ESC(object):
    """ """

    def __init__(self, battery, weight):
        """ """

        self.battery = battery
        self.weight = weight
        self.throttle = 0
        self.consumption = 0 # TODO, how much idle current?

    def set_throttle(self, value):
        """ """

        self.throttle = value
        if value < 0:
            self.throttle = 0
        elif value > 100:
            self.throttle = 100

        # TODO, set consumption
