
"""
Fault-Tolerant Quadcopter - telemetry stream management
"""

# built-in
import sys
import logging

class StreamManager():
    """ Manage IO streams. """

    log = logging.getLogger(__name__)

    def __init__(self, name, debug=True):
        """ Initialize stream data. """

        self.output_streams = {}
        self.debug = debug
        self.name = name
        self.message_count = 0

    def remove_output(self, name):
        """ Remove an output stream. """

        self.output_streams.pop(name, None)

    def add_output(self, name, io_obj):
        """ Add an output stream. """

        self.output_streams[name] = io_obj

    def write(self, message):
        """ Write a message to all output streams. """

        if self.debug:
            StreamManager.log.info("%s-stream: %s", self.name, str(message))
        for _, stream in self.output_streams.items():
            stream.write(message)
        self.message_count += 1

    def report(self):
        """ Report current metrics of this stream manager. """

        StreamManager.log.info("%s-stream: %d messages", self.name,
                               self.message_count)
