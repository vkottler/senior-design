
"""
Fault-Tolerant Quadcopter - telemetry channel
"""

# internal
import json

class Channel(object):
    """ """

    @staticmethod
    def channel_from_json(json_data):
        """ """
        pass

    def __init__(self, name, value_type, unit, telemetry_client):
        """ """

        self.dict = {}
        self.dict["name"]  = name
        self.dict["type"]  = str(value_type)
        self.dict["unit"]  = unit
        self.dict["value"] = None
        self.type = value_type
        self.telemetry_client = telemetry_client

    def set_data(self, data):
        """ """

        self.dict["value"] = data

    def to_json(self, pretty=False):
        """ """

        return json.dumps(self.dict, ensure_ascii=True)

    def to_byte():
        """ """
        pass

    def publish(self):
        """ """
        pass
