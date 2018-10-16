
"""
Fault-Tolerant Quadcopter - telemetry buffer storage
"""

# built-in
import time

# internal
from .ChannelBuffer import ChannelBuffer

class TelemetryBuffer(object):
    """ """

    def __init__(self):
        """ """

        self.channels = {}

    def get_known_channels(self):
        """ """

        return self.channels.keys()

    def remove(self, channel_name):
        """ """

        self.channels.pop(channel_name, None)

    def put(self, channel):
        """ """

        name = channel.dict["name"]
        if name not in self.channels:
            self.channels[name] = ChannelBuffer(channel)
        self.channels[name].capture()

    def channel_age(self, channel_name):
        """ """

        if channel_name not in self.channels:
            return -1
        return (time.time() * 1000) - self.channels[channel_name].get_newest()[1]

    def channel_ages(self):
        """ """

        ages = {}
        for channel_name in self.get_known_channels():
            ages[channel_name] = self.channel_age(channel_name)
        return ages

    def get(self, channel_names, start_idx=0, num_elems=-1):
        """ """
        pass
