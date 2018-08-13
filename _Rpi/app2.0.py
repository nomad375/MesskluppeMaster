#!/usr/bin/env python
#============================================================================#
#   Includes
#============================================================================#
#Flask
from flask import *
import jinja2.exceptions
from flask_socketio import SocketIO
#Others
from messkluppe_nrf24 import *
import csv
import os.path
#============================================================================#
#   lib_nrf24 Setup
#============================================================================#
import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
global pipes
global radio
pipes = [0xAB, 0xCD, 0xAB, 0xCD, 0x71]
radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 22)
radio.setPayloadSize(32)
radio.setChannel(77)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_LOW)
radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()
radio.setCRCLength(NRF24.CRC_8)
radio.openReadingPipe(1,pipes)
radio.startListening()
#============================================================================#
#   Flask Routs
#============================================================================#
app = Flask(__name__)
app.config['SECRET_KEY'] = 'vnkdjnfjknfl1232#'
socketio = SocketIO(app)

@app.route('/')
def index():
    change_clip_modus(0)
    return render_template('index.html')

#============================================================================#
#   start Flask Server
#============================================================================#
@app.before_first_request
def activate_job():		
    t = threading.Thread(target=com_clip)
    t.start()
    
if __name__ == '__main__':
    socketio.run(app, debug=True, host="0.0.0.0")