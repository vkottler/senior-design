
"""
Fault-Tolerant Quadcopter - simulated battery
"""

CAPACITY_LUT = {}
CAPACITY_LUT[100] = 4.20
CAPACITY_LUT[95]  = 4.15
CAPACITY_LUT[90]  = 4.11
CAPACITY_LUT[85]  = 4.08
CAPACITY_LUT[80]  = 4.02
CAPACITY_LUT[75]  = 3.98
CAPACITY_LUT[70]  = 3.95
CAPACITY_LUT[65]  = 3.91
CAPACITY_LUT[60]  = 3.87
CAPACITY_LUT[55]  = 3.85
CAPACITY_LUT[50]  = 3.84
CAPACITY_LUT[45]  = 3.82
CAPACITY_LUT[40]  = 3.80
CAPACITY_LUT[35]  = 3.79
CAPACITY_LUT[30]  = 3.77
CAPACITY_LUT[25]  = 3.75
CAPACITY_LUT[20]  = 3.73
CAPACITY_LUT[15]  = 3.71
CAPACITY_LUT[10]  = 3.69
CAPACITY_LUT[5]   = 3.61
CAPACITY_LUT[0]   = 3.27

class Battery(object):
    """ Simulated Lithium-Ion Polymer (LiPo) cell configurations. """

    def __init__(self, cell_count, capacity_mAh, internal_res_mOhm, weight_g,
                 temperature=25):
        """ Constructor, some parameters are specified by the caller. """

        self.cell_count = cell_count
        self.starting_capacity = capacity_mAh
        self.capacity = capacity_mAh
        self.weight = weight_g
        self.temperature = temperature
        self.internal_res = internal_res_mOhm
        self.voltages = [CAPACITY_LUT[100] for i in range(self.cell_count)]

        # https://www.nrel.gov/transportation/assets/pdfs/long_beach_btm.pdf
        self.specific_heat = 1011.8 # J/kg per deg C

    def consume_capacity(self, load_amps, time):
        """ """

        # convert load-time pair into mAh:
        energy = (load_amps * 1000)

        # TODO

        pass

    def request_power(self, time):
        """ """

        # TODO

        pass

    def get_voltage(self, load_amps=0):
        """
        Calculate the voltage based on the state of capacity and specified
        load (in Amperes).
        """

        charge = self.percent_charge(True)
        interp = charge % 5
        if interp != 0 and interp >= 3:
            charge += 5 - interp
        elif interp != 0:
            charge -= interp
        return (self.cell_count * CAPACITY_LUT[charge]) - (self.internal_res * load / 1000)

    def percent_charge(self, round_result=False):
        """
        Calculate the percentage of capacity remaining based on the state
        of capacity.
        """

        result = 100 * (self.capacity / self.starting_capacity)
        if round_result:
            return round(result)
        return result
