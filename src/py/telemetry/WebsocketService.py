
"""
Fault-Tolerant Quadcopter - websocket service wrapper
"""

# built-in
import asyncio
import logging

# internal
from .Daemon import Daemon

# third-party
import websockets

@asyncio.coroutine
def WebsocketHandler(websocket, path):
    """ Handle a websocket connection. """

    WebsocketService.log_client_action(websocket.remote_address, "connected")

    while True:
        try:
            message = yield from websocket.recv()
        except websockets.exceptions.ConnectionClosed:
            WebsocketService.log_client_action(websocket.remote_address,
                                               "closed connection")
            return

        print("websocket: " + message)
        yield from websocket.send(message)

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

    def __init__(self, port, name="websocket"):
        """ Initialize the websocket server daemon. """

        self.name = name
        self.port = port

        loop = asyncio.new_event_loop()
        asyncio.set_event_loop(loop)

        server = websockets.serve(WebsocketHandler, '', port)
        run_fn = self.start_generator(server, loop)
        stop_fn = self.stop_generator(loop)
        super().__init__(self.name, run_fn, stop_fn)
