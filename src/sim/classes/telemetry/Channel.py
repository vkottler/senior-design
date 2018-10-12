
"""
Fault-Tolerant Quadcopter - telemetry channel
"""

class Channel(object):
    """ """

    @staticmethod
    def channel_from_json(json_data):
        """ """
        pass

    def __init__(self, name, value_type, unit, telemetry_client):
        """ """

        self.name  = name
        self.type  = value_type
        self.unit  = unit
        self.value = None
        self.telemetry_client

    def to_json(self):
        """ """

    # TODO: add a "to_json" function
    # TODO: add a "publish" function
    # TODO: add static class to return a Channel object from JSON
