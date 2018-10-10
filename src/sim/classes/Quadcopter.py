
"""
Fault-Tolerant Quadcopter - simulated full quadcopter
"""

from .Battery import Battery
from .ESC import ESC
from .Motor import

class Quadcopter(object):
    """ """

    def __init__(self):
        """ """

        self.chassis_weight  = 24     # structural pcb (top)
        self.chassis_weight += 41     # structural pcb (bottom)
        self.chassis_weight += 58 * 4 # motor limb(s)

        self.battery = Battery(3, 1300, 116)
        self.escs = [ESC(self.battery, 31),
                     ESC(self.battery, 31),
                     ESC(self.battery, 31),
                     ESC(self.battery, 31)]
        self.motors = [Motor( 1,  0, self.escs[0], 59),
                       Motor(-1,  0, self.escs[0], 59),
                       Motor( 0,  1, self.escs[0], 59),
                       Motor( 0, -1, self.escs[0], 59)]
