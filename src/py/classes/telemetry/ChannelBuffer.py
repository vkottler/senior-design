
"""
Fault-Tolerant Quadcopter - channel buffer storage
"""

# built-in
from collections import deque

# internal

class ChannelBuffer(object):
    """ """

    DEPTH = 64

    def __init__(self, channel):
        """ """

        self.channel = channel
        self.items = 0
        self.buffer = deque(maxlen=ChannelBuffer.DEPTH)

    def capture(self):
        """ """

        self.buffer.append((self.channel.dict["value"], self.channel.dict["timestamp"]))
        if self.items < ChannelBuffer.DEPTH:
            self.items += 1

    def getall(self, clear=False):
        """ """

        items = list(self.buffer)
        if clear:
            self.buffer.clear()
            self.items = 0
        return items

    def get_newest(self):
        """ """

        return self.buffer.index(self.items - 1)

    def pop_oldest(self):
        """ """

        data = self.buffer.popleft()
        self.items -= 1
        return data
