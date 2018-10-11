
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

        # internal resistance setup:
        # open-circuit voltage: 11.54 V
        # under-load   voltage: 11.26 V
        # under-load   current:  4.20 A
        #                 load:  2.8  Ohms
        # voltage  /   current:  0.28 V / 4.20 A = 0.0667 Ohms
        # total res. / # cells: 66.67 mOhm / 3 = 22.22 mOhm
        self.battery = Battery(3, 1300, 22.22, 116)

        self.escs = [ESC(self.battery, 31),
                     ESC(self.battery, 31),
                     ESC(self.battery, 31),
                     ESC(self.battery, 31)]
        # TODO, measure coordinate system
        self.motors = [Motor( 1,  0, self.escs[0], 59),
                       Motor(-1,  0, self.escs[0], 59),
                       Motor( 0,  1, self.escs[0], 59),
                       Motor( 0, -1, self.escs[0], 59)]
