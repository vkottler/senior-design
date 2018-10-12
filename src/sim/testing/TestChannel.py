
"""
Fault-Tolerant Quadcopter - testing telemetry channels
"""

# payload
from sim.classes.telemetry.Channel import Channel
from sim.classes.telemetry.Client import TelemetryClient
from sim.classes.telemetry.Server import TelemetryServer

# built-in
import unittest

class TestChannel(unittest.TestCase):
    """ """

    def test_channel_from_json(self):
        """ """

        # TODO
        self.assertTrue(True)

    def test_to_json(self):
        """
        Test that a conversion to and from JSON results in an 'equivalent'
        object.
        """

        json_string = self.chan.to_json()
        check_channel = Channel.from_json(json_string, self.client)
        self.assertTrue(self.chan.equals(check_channel, True))

    def test_publish(self):
        """ """

        self.chan.publish()

    def setUp(self):
        """ """

        # set up client and server pair
        self.client = TelemetryClient()
        self.server = TelemetryServer()
        self.server.start()
        self.client.start()

        self.chan = Channel("test", int, "meters", self.client, 123)

    def tearDown(self):
        """ """

        self.client.stop()
        self.server.stop()
