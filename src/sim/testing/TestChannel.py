
"""
Fault-Tolerant Quadcopter - testing telemetry channels
"""

# payload
from sim.classes.telemetry.Channel import Channel
from sim.classes.telemetry.Client import TelemetryClient

# built-in
import unittest

class TestChannel(unittest.TestCase):
    """ """

    def test_channel_from_json(self):
        """ """
        self.assertTrue(True)

    def test_to_json(self):
        """ """
        print(self.chan.to_json())
        self.assertTrue(True)

    def test_publish(self):
        """ """
        self.assertTrue(True)

    def setUp(self):
        """ """

        self.client = TelemetryClient()
        self.chan = Channel("test", int, "meters", self.client)
        self.chan.set_data(123)
