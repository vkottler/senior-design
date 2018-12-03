
"""
Fault-Tolerant Quadcopter - multi-thread management
"""

# built-in
import logging
import threading

class Daemon():
    """ Run loops in thread contexts that can be stopped. """

    log = logging.getLogger(__name__)

    def __init__(self, name, start_fn, stop_fn):
        """ Set up the thread object and initialize instance members. """
        
        self.name = name
        self.stop_fn = stop_fn
        self.thread = threading.Thread(target=start_fn)
        self.started = False
        self.stopped = False

    def start(self):
        """ Run 'run_fn' in a new thread context. """

        if self.started:
            Daemon.log.error("'%s' can't be started again.", self.name)
            return False
        self.thread.start()
        self.started = True
        return True

    def stop(self):
        """ Call 'stop_fn' and wait for the thread to terminate. """

        if not self.started:
            Daemon.log.error("'%s' can't be stopped, isn't started.", self.name)
            return False
        if self.stopped:
            Daemon.log.error("'%s' can't be stopped again.", self.name)
            return False
        self.stopped = True
        self.stop_fn()
        self.thread.join()
        return True
