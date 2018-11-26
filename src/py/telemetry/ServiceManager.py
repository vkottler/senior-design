
"""
Fault-Tolerant Quadcopter - telemetry service management
"""

# built-in
import logging
from time import sleep

class ServiceManager():
    """ Manage the execution of telemetry services. """

    log = logging.getLogger(__name__)

    def __init__(self):
        """ Initialize a list of services. """

        self.services = []

    def add_service(self, service):
        """ Add a service. """

        self.services.append(service)

    def start(self):
        """ Start all services. """

        for service in self.services:
            if not service.start():
                ServiceManager.log.warn("A service could not start.")
                return False
        ServiceManager.log.info("All services started.")
        return True

    def stop(self):
        """ Stop all services. """

        for service in self.services:
            if not service.stop():
                ServiceManager.log.warn("A service could not stop.")
                return False
        ServiceManager.log.info("All services stopped.")
        return True

    def run_forever(self):
        """
        Sleep this thread until the user triggers an interrupt,
        then stop and return.
        """

        if not self.start():
            return False

        while True:
            try:
                sleep(10000)
            except KeyboardInterrupt:
                break

        if not self.stop():
            return False

        return True
