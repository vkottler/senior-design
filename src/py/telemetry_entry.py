
"""
Fault-Tolerant Quadcopter - telemetry services
"""

# built-in
import argparse
import logging

# internal
from .telemetry.ServiceManager import ServiceManager
from .telemetry.HttpService import HttpService
from .telemetry.TcpDataService import TcpDataService
from .telemetry.TcpCommandService import TcpCommandService

def main(argv):
    """ Telemetry service hosting entry. """

    logging.basicConfig(level=logging.INFO)

    desc = "Flight data publish and subscribe broker."
    parser = argparse.ArgumentParser(description=desc, prog="telemetry.py")

    # initialize arguments
    parser.add_argument("--ws-port", type=int, default=10020, help="")
    parser.add_argument("--tcp-data-port", type=int, default=9020, help="")
    parser.add_argument("--tcp-cmd-port", type=int, default=9091, help="")
    parser.add_argument("--http-port", type=int, default=8080, help="")

    # parse arguments
    args = parser.parse_args(argv[1:])

    # add services
    manager = ServiceManager()
    manager.add_service(HttpService(args.http_port))
    manager.add_service(TcpDataService(args.tcp_data_port))
    manager.add_service(TcpCommandService(args.tcp_cmd_port))

    # run all services
    if not manager.run_forever():
        return 1
    return 0
