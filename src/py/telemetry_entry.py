
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
from .telemetry.WebsocketService import WebsocketService

from .telemetry.StreamManager import StreamManager

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
    parser.add_argument("-s", "--silent", action="store_true", help="")

    # parse arguments
    args = parser.parse_args(argv[1:])

    telemetry_stream = StreamManager("telemetry", not args.silent)
    command_stream = StreamManager("command", not args.silent)

    # add services
    manager = ServiceManager()
    #manager.add_service(HttpService(args.http_port))
    manager.add_service(TcpDataService(args.tcp_data_port, telemetry_stream,
                                       command_stream))
    #manager.add_service(TcpCommandService(args.tcp_cmd_port, command_stream))
    manager.add_service(WebsocketService(args.ws_port, telemetry_stream,
                                         command_stream))

    # run all services
    result = 0
    if not manager.run_forever():
        result = 1
    telemetry_stream.report()
    command_stream.report()
    return result
