
"""
Fault-Tolerant Quadcopter - command-line entry and argument parsing
"""

# built-in
import argparse
import logging

# internal
from .sim import simulate

def sim_main(argv):
    """ """

    logging.basicConfig(level=logging.INFO)

    desc = "Flight Algorithm Tuning and Testing"
    parser = argparse.ArgumentParser(description=desc, prog="sim.py")

    # optional arguments
    #parser.add_argument("-i", "--interactive",
    #                    help="Run this tool interactively",
    #                    action="store_true")
    parser.add_argument("-t", "--real-time",
                        help="Run in real time instead of "
                             "at full compute speed",
                        action="store_true")
    parser.add_argument("-d", "--duration",
                        help="Number of seconds to run for",
                        type=int, default=5)
    parser.add_argument("-s", "--step-duration",
                        help="Number of milliseconds per step",
                        type=int, default=1)

    # parse input arguments
    args = parser.parse_args(argv[1:])

    return simulate(args)
