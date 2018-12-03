
"""
Fault-Tolerant Quadcopter - http telemetry services
"""

# built-in
from http.server import BaseHTTPRequestHandler, HTTPServer

# internal
from .TelemetryService import TelemetryService

class HttpHandler(BaseHTTPRequestHandler):
    """ Handle client requests. """

    def do_HEAD(self):
        """ Handle a 'HEAD' command. """

        # TODO
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'HEAD response')

    def do_GET(self):
        """ Handle a 'GET' command. """

        # TODO
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'GET response')

    def do_POST(self):
        """ Handle a 'POST' command. """

        # TODO
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'POST response')

class HttpService(TelemetryService):
    """ A service for dispatching potential http client requests. """

    def __init__(self, port):
        """
        Initializes via the parent constructor and http-specific
        server-handler pair.
        """

        self.address = ("", port)
        HTTPServer.allow_reuse_address = True
        self.server = HTTPServer(self.address, HttpHandler)
        super().__init__("http", self.server, str(port))
