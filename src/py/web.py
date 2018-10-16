
"""
Fault-Tolerant Quadcopter - telemetry web services
"""

from flask import Flask

app = Flask(__name__)

@app.route("/")
def hello():
        return "Hello World!"
