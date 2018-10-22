
"""
Fault-Tolerant Quadcopter - RESTful services for interacting with telemetry
"""

# third-party
from flask import Flask, render_template
from werkzeug.serving import make_server

# built-in
import logging
import threading

# internal

class TelemetryService(object):
    """ Web API for telemetry metrics and setting up streams. """

    log = logging.getLogger(__name__)

    def index(self):
        """ Landing page for the Telemetry API. """

        return render_template("index.html", name=self.name, port=self.port)

    def __init__(self, telemetry_buffer, port=8000, name="Telemetry API"):
        """ Constructor. """

        self.buffer = telemetry_buffer
        self.port = port
        self.name = name
        self.started = False
        self.stopped = False

        # initialize flask
        self.app = Flask(__name__)
        self.app.add_url_rule('/', 'index', self.index)

        # set up the traffic serving thread
        self.server = make_server('127.0.0.1', self.port, self.app)
        self.thread = threading.Thread(target=self.server.serve_forever)

    def start(self):
        """ Start serving the telemetry API. """

        # check to see if starting this service makes sense
        if self.started:
            TelemetryService.log.error("'%s' can't be started.", self.name)
            return False

        self.thread.start()
        TelemetryService.log.info("'%s' was started on port %d.",
                                  self.name, self.port)
        self.started = True
        return True

    def stop(self):
        """ Stop serving the telemetry API. """

        # check to see if stopping this service makes sense
        if not self.started or self.stopped:
            TelemetryService.log.error("'%s' can't be stopped.", self.name)
            return False

        self.stopped = True
        self.server.shutdown()
        self.thread.join()
        TelemetryService.log.info("'%s' was stopped.", self.name)
        return True
