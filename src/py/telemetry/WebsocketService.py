
"""
Fault-Tolerant Quadcopter - websocket service wrapper
"""

# built-in
import asyncio
import logging

# internal
from .Daemon import Daemon
from .TelemetryService import TelemetryService

# third-party
import websockets

class WebsocketWriter():
    """ Thin class so that we can have an object with a 'write' attribute. """

    def __init__(self, write_fn):
        """ Constructor. """

        self.write = write_fn

class WebsocketService(Daemon):
    """ Websocket server daemon. """

    log = logging.getLogger(__name__)

    @staticmethod
    def log_client_action(client, action):
        """ Convenient logging abstraction. """

        WebsocketService.log.info("websocket client %s: %s:%d", action,
                                  client[0], client[1])

    def start_generator(self, server, loop):
        """ Generate a startup function for a websocket server. """

        def start_fn():
            """ Start the websocket server. """

            WebsocketService.log.info("'%s' sevice started (%s).",
                                      self.name, self.port)
            loop.run_until_complete(server)
            loop.run_forever()

        return start_fn

    def stop_generator(self, loop):
        """ Generate a shutdown function for a websocket server. """

        def stop_fn():
            """ Stop the websocket server. """

            WebsocketService.log.info("'%s' sevice stopped (%s).",
                                      self.name, self.port)
            loop.call_soon_threadsafe(loop.stop)
            while loop.is_running():
                pass
            loop.call_soon_threadsafe(loop.close)

        return stop_fn

    def handler_generator(self, telemetry_stream, command_stream, loop):
        """
        Generate a websocket handler that will work with the websockets
        package.
        """

        @asyncio.coroutine
        def handler(websocket, path):
            """ Handle a websocket connection. """

            WebsocketService.log_client_action(websocket.remote_address, "connected")

            # set this client up as a telemetry subscriber
            def websocket_writer(message):
                string_msg = message.decode("utf-8")
                asyncio.run_coroutine_threadsafe(websocket.send(string_msg),
                                                 loop=loop)
            client_str = TelemetryService.client_address_str(websocket.remote_address)
            telemetry_stream.add_output(client_str, WebsocketWriter(websocket_writer))

            # forward incoming client data to the command stream 
            while True:
                try:
                    message = yield from websocket.recv()
                except websockets.exceptions.ConnectionClosed:
                    WebsocketService.log_client_action(websocket.remote_address,
                                                       "closed connection")
                    telemetry_stream.remove_output(client_str)
                    return

                command_stream.write(message.encode("utf-8"))

        return handler

    def __init__(self, port, telemetry_stream, command_stream, name="websocket"):
        """ Initialize the websocket server daemon. """

        self.name = name
        self.port = port

        loop = asyncio.new_event_loop()
        asyncio.set_event_loop(loop)

        handler = self.handler_generator(telemetry_stream, command_stream, loop)

        server = websockets.serve(handler, '', port)
        run_fn = self.start_generator(server, loop)
        stop_fn = self.stop_generator(loop)
        super().__init__(self.name, run_fn, stop_fn)
