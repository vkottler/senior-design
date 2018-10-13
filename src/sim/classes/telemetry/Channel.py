
"""
Fault-Tolerant Quadcopter - telemetry channel
"""

# internal
import json

class ChannelException(Exception):
    """ Custom exception for Channel-related events. """
    pass

class Channel(object):
    """
    Object for encapsulating data that can be transported as telemetry.
    """

    @staticmethod
    def from_json(json_data, telemetry_client):
        """ Create a channel from a JSON String. """

        json_dict = json.loads(json_data)

        # convert type string back into the actual type
        json_type = int
        if json_dict["type"] == "float":
            json_type = float
        elif json_dict["type"] == "str":
            json_type = str

        return Channel(json_dict["name"], json_type,
                       json_dict["unit"], telemetry_client,
                       json_dict["value"])

    def __init__(self, name, value_type, unit, telemetry_client,
                 value=None):
        """ Initialize a channel. """

        self.dict = {}
        self.dict["name"]  = name
        self.dict["type"]  = value_type.__name__
        self.dict["unit"]  = unit
        self.dict["value"] = value
        self.type = value_type
        self.telemetry_client = telemetry_client

    def equals(self, other_channel, check_value=False):
        """ Check if two channel objects are equal. """

        if self.dict["name"] != other_channel.dict["name"]:
            return False
        elif self.dict["type"] != other_channel.dict["type"]:
            return False
        elif self.dict["unit"] != other_channel.dict["unit"]:
            return False
        if check_value and self.dict["value"] != other_channel.dict["value"]:
            return False
        return True

    def set_data(self, data, publish=False):
        """ Set a new value for this channel. """

        self.dict["value"] = data
        if publish:
            self.publish()

    def to_json(self):
        """ Convert this channel to JSON. """

        return json.dumps(self.dict, sort_keys=True)

    def publish(self):
        """
        Publish this channel's data as telemetry via its telemetry client.
        """

        if self.telemetry_client is None: 
            raise ChannelException("Telemetry client is None for this channel.")
        if not self.telemetry_client.started or self.telemetry_client.stopped:
            raise ChannelException("Telemetry client not active.")
        self.telemetry_client.data.put(self)
