
"""
Fault-Tolerant Quadcopter - command-line entry and argument parsing
"""

# built-in
import argparse

# internal
from .classes.SimulationEngine import SimulationEngine

def simulate():
    """ """

    return 0

def main(argv):
    """ """

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
                        type=int, default=10)
    parser.add_argument("-s", "--step-duration",
                        help="Number of milliseconds per step",
                        type=int, default=1)

    # parse input arguments
    args = parser.parse_args(argv[1:])

    # initialize the simulation environment
    engine = SimulationEngine(args.step_duration, args.real_time)

    # run the simulation to completion
    time_remaining = args.duration * 1000
    while time_remaining:
        engine.step()
        time_remaining -= args.step_duration

    return 0
