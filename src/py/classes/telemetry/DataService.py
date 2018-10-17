
"""
Fault-Tolerant Quadcopter - RESTful services for interacting with telemetry
"""

# third-party
from flask import Flask
from flask_restful import Resource, Api

# built-in

# internal

class TelemetryService(object):
    """ """

    def __init__(self, port, telemetry_buffer):
        """ """

        self.buffer = telemetry_buffer

        self.app = Flask(__name__)
        self.api = Api(self.app)

        self.thread = threading.Thread(target=self.app.run,
                                       kwargs={"port": port})

    def start(self):
        """ """

        self.thread.start()

    def stop(self):
        """ """

        self.thread.join()
