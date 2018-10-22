
"""
Fault-Tolerant Quadcopter - testing telemetry channels
"""

# payload
from py.classes.telemetry.Channel import Channel
from py.classes.telemetry.Client import TelemetryClient
from py.classes.telemetry.Server import TelemetryServer

# built-in
import unittest

class TestChannel(unittest.TestCase):
    """ Test Channel-related actions. """

    def test_to_from_json(self):
        """
        Test that a conversion to and from JSON results in an 'equivalent'
        object.
        """

        json_string = self.chan.to_json()
        check_channel = Channel.from_json(json_string, self.client)
        self.assertTrue(self.chan.equals(check_channel, True))

    def test_publish(self):
        """ Test publishing a channel. """

        for i in range(1000):
            self.chan.set_data(i, True)

    def setUp(self):
        """ Initialize client-server pair and a single channel. """

        # set up client and server pair, use an arbitrary port
        self.server = TelemetryServer(0)
        self.server.start()
        self.client = TelemetryClient(self.server.port())
        self.client.start()

        self.chan = Channel("test", int, "meters", self.client, 123)

    def tearDown(self):
        """ Stop client and server. """

        self.client.stop()
        self.server.stop()
