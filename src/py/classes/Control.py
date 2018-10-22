
"""
Fault-Tolerant Quadcopter - Python classes representing control.h structs/types
"""

# built-in
from ctypes import *

# internal

class ImuData(Structure):
    """
    In-memory data structure (same as over the wire from the sensor) for the
    IMU.
    """

    # coerce half-word alignment
    _pack_ = 2
    _fields_ = [("ax", c_short), ("ay", c_short), ("az", c_short)]

class GyroData(Structure):
    """
    In-memory data structure (same as over the wire from the sensor) for the
    gyro.
    """

    # coerce half-word alignment
    _pack_ = 2
    _fields_ = [("vx", c_short), ("vy", c_short), ("vz", c_short)]

class MotionConfiguration(Structure):
    """ """

    _fields_ = [("imu_rate", c_float),          ("gyro_rate", c_float),
                ("imu_sample_period", c_float), ("gyro_sample_period", c_float),
                ("imu_lsb", c_float),           ("gyro_lsb", c_float)]

class MotionState(Structure):
    """ """

    _fields_ = [("imu_roll_len", c_uint), ("gyro_roll_len", c_uint),
                ("imu_curr", c_uint),     ("gyro_curr", c_uint),

                # pointers to rolling buffers
                ("roll_ax", POINTER(c_short)),
                ("roll_ay", POINTER(c_short)),
                ("roll_az", POINTER(c_short)),
                ("roll_gx", POINTER(c_short)),
                ("roll_gy", POINTER(c_short)),
                ("roll_gz", POINTER(c_short)),

                # averaged measurements
                ("avg_ax", c_short), ("avg_ay", c_short), ("avg_az", c_short),
                ("avg_gx", c_short), ("avg_gy", c_short), ("avg_gz", c_short),

                # accumulated measurements
                ("acc_ax", c_short), ("acc_ay", c_short), ("acc_az", c_short),
                ("acc_gx", c_short), ("acc_gy", c_short), ("acc_gz", c_short)]
