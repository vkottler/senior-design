
"""
Fault-Tolerant Quadcopter - simulated battery
"""

# https://www.rcgroups.com/forums/showthread.php?2366548-LiPo-80-rule-and-my-shallow-charge-rates
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

class BatteryException(Exception):
    """ Custom exception for Battery-related events. """
    pass

class Battery(object):
    """ Simulated Lithium-Ion Polymer (LiPo) cell configurations. """

    def __init__(self, cell_count_s, cell_count_p, capacity_mAh,
                 internal_res_mOhm, weight_g, temperature=25):
        """ Constructor, some parameters are specified by the caller. """

        self.cell_count_s = cell_count_s
        self.starting_capacity = capacity_mAh * cell_count_p
        self.capacity = self.starting_capacity
        self.weight = weight_g
        self.temperature = temperature
        self.internal_res = internal_res_mOhm
        self.voltages = [CAPACITY_LUT[100] for i in range(self.cell_count_s)]

        # https://www.nrel.gov/transportation/assets/pdfs/long_beach_btm.pdf
        self.specific_heat = 1011.8 * (self.weight / 1000) # J/kg per deg C

    def add_heat(load_amps, time_s):
        """
        Account for heat rise experienced at some load current for a
        specified duration.
        """

        heat_rise_energy = self.voltage_drop(load_amps) * load_amps * time_s
        self.temperature += heat_rise_energy / self.specific_heat

    def time_remaining_at_load(load_amps):
        """
        Determine how many seconds the battery will run for at a specified
        load from its current capacity.
        """

        return (self.capacity * 3600) / (load_amps * 1000)

    def consume_capacity(self, load_amps, time_s):
        """
        Subtract from the state of charge based on current consumed for
        a specified duration.
        """

        # convert load-time pair into mAh
        charge_used = (load_amps * 1000) * (time_s / 3600)

        # prevent using an amount of charge that would make the simulation
        # environment inaccurate / unstable
        if charge_used >= (self.starting_capacity * 0.05):
            raise BatteryException("Cannot use 5% or more of battery capacity at once.")

        self.capacity -= charge_used

        # prevent execution from continuing if no capacity remains
        if self.capacity <= 0:
            raise BatteryException("Capacity reached 0.")

        # account for heat rise
        self.add_heat(load_amps, time_s)

    def request_power(self, power_w, time_s, consume=True):
        """
        Consume power from the battery for a specified duration.

        Returns the following tuple:
            (resulting_voltage, resulting_current, time_remaining_at_power)
        """

        # P = IV
        # V depends on I
        # V = V(I) = v(0) - v(I), v(0) is a constant
        # P = (v(0) - v(I)) * I
        # ...solving a differential equation
        #
        # P is the area under the curve of V(I)
        #
        # Approximate V and I by "integrating" with a discrete step size
        # until the area is greater than or equal to the requested power.
        amps = 0
        dI = 0.01
        accumulated_power_w = 0
        while accumulated_power_w < power_w:
            accumulated_power_w += self.get_voltage(amps) * dI
            amps += dI

        # Consume battery capacity
        if consume:
            self.consume_capacity(amps, time_s)

        # note that if energy was consumed, time remaining here is up to date
        return (self.get_voltage(amps), amps, self.time_remaining_at_load(amps))

    def voltage_drop(load_amps):
        """
        Calculate the voltage drop due to internal resistance for the
        specified load.
        """

        return self.internal_res * load_amps / 1000

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
        return (self.cell_count_s * CAPACITY_LUT[charge]) - self.voltage_drop(load_amps)

    def percent_charge(self, round_result=False):
        """
        Calculate the percentage of capacity remaining based on the state
        of capacity.
        """

        result = 100 * (self.capacity / self.starting_capacity)
        if round_result:
            return round(result)
        return result
