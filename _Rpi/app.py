#!/usr/bin/env python

#============================================================================#
#   lib_nrf24
#============================================================================#
import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode(GPIO.BCM)

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
#============================================================================#
#   translate radio Msq from byte to int
#============================================================================#
def translate_from_radio(msg, size):
    translated_msg=[]
    for i in range (0, size, 4):
        translated_msg.append(int.from_bytes([msg[i+3], msg[i+2], msg[i+1], msg[i]], byteorder='big')) 
        
    #print("Translate FROM Radio: " + str(msg) + " --> " + str(translated_msg))
    return translated_msg
#============================================================================#
#============================================================================#
#   Split the msg element in 4 bytes and add it to translated msg
#============================================================================#   
def translate_to_radio(msg):
    translated_msg=[]
    for i in range (0, len(msg)):
        x=msg[i].to_bytes(4, byteorder='big')
        for g in reversed(x):
            translated_msg.append(g)        
        
    #print("Translate TO Radio: " + str(msg) + " --> " + str(translated_msg))
    return translated_msg
#============================================================================#
#============================================================================#
#   Seperates the Clip ID and the Task | idTask[ID, Task] ID = 22 * 1000 + Task
#============================================================================#
def idTask (idTask):
    if type(idTask) is int:
        create_task = int(idTask%1000)
        create_id = int((idTask-create_task)/1000)
        new = [create_id, create_task]
        return new
		
    if type(idTask) is list:
        new = idTask[0] * 1000 + idTask[1]
        return new
        
#============================================================================#

        

#Flask
from flask import *
import jinja2.exceptions

#Others
from subprocess import call
from random import sample

#============================================================================#
#   global variables
#============================================================================#
global clip_modus
clip_modus = 0
g_ping = 50





app = Flask(__name__)
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/<pagename>')
def admin(pagename):
    return render_template(pagename+'.html')

@app.route('/<path:resource>')
def serveStaticResource(resource):
	return send_from_directory('static/', resource)

    
    
#============================================================================#
#   ping the clip Test
#============================================================================# 
def clip_ping_test():
    receivedMessage = [0, 0, 0, 0, 0, 0, 0, 0,]
    RcvMsg = [0, 0, 0, 0, 0, 0, 0, 0,]
    SndMsg=[0, 0, 0, 0, 0, 0, 0, 0,]
    last_timestamp = 0
    while 1:
        if (radio.available()):
            size = radio.getDynamicPayloadSize()
            radio.read(receivedMessage, size)
            #print(str(receivedMessage))
            RcvMsg = translate_from_radio(receivedMessage, size)
            #print (str(RcvMsg))
            print ("RcvMsg " + str(RcvMsg))
            
            # Lets check what we get
            got_idTask = idTask(RcvMsg[0])

            if (got_idTask[0] > 0 and got_idTask[0] < 100 and got_idTask[1] < 100):
                #What I got looks good so check the ping
                ping = RcvMsg[1] - last_timestamp
                last_timestamp = RcvMsg[1]
                print ("ping: " + str(ping))
                if (ping < 100 and ping > 0):
                
                    # Ping is good -> answer

                    SndMsg = [RcvMsg[0], RcvMsg[1], int(time.time()),0,RcvMsg[5],0,0,0]
                    SndMsg = translate_to_radio(SndMsg)
                    radio.writeAckPayload(1, SndMsg, len(SndMsg))
                    
                else:
                    # Bad Ping
                    print(":(")
                    
                
                
            
           
            
            #print ("RcvMsg " + str(RcvMsg) + " SndMsg " +str(translate_from_radio(SndMsg, len(SndMsg))))
        #else:
            #time.sleep(2)

#============================================================================#
#   ping the clip
#============================================================================# 

def clip_ping():
    
    start_time = int(round(time.time() * 1000))
    act_time = start_time
    time_from_arduino = 0
    receivedMessage=[]   
    SndMsg=[0, 0, 0, 0, 0, 0, 0, 0,]
    radio.writeAckPayload(1, SndMsg, len(SndMsg))    
    
    
    while 1:
        start_time = int(round(time.time() * 1000))
        act_time = start_time
        
        while (act_time - start_time < 500):
            act_time = int(round(time.time() * 1000))
            #print("Act_Time = " + str(act_time) + " Start Time = " + str(start_time) + "->> " + str(act_time-start_time))

            
            if (radio.available()):
                size = radio.getDynamicPayloadSize()
                radio.read(receivedMessage, size)
                print("receivedMessage: " + str(receivedMessage))
                RcvMsg = translate_from_radio(receivedMessage, size)
                if RcvMsg[0] != 0:
                    old_idTask = idTask(RcvMsg[0])
                    new_idTask = idTask([old_idTask[0], clip_modus])
                    
                    SndMsg = [new_idTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                    SndMsg = translate_to_radio(SndMsg)
                    radio.writeAckPayload(1, SndMsg, len(SndMsg))
                
                    print ("RcvMsg " + str(RcvMsg) + " SndMsg " +str(translate_from_radio(SndMsg, len(SndMsg))))
                
                    ping = RcvMsg[1] - time_from_arduino
            
                
                   # if (ping < 100 and ping > 0):
                    # return str(ping)
                # else:
                    # time_from_arduino = RcvMsg[1]
            
        #else:
           # print ("Timeout!")
            #ping = -1
           # return str(ping)
#============================================================================# 

@app.route('/start_logging')
def start_logging():
	change_clip_modus(20)
	return str(clip_modus)	
	
@app.route('/stop_logging')
def stop_logging():
	change_clip_modus(0)
	return str(clip_modus)	

@app.route('/get_globals')
def get_globals():	
	return jsonify(
		clip_modus = clip_modus,
		silvo	= 1)
        
@app.route('/ping')
def ping():
	ping = 10
	return ping	
	
def change_clip_modus(new):
	global clip_modus
	print("change_clip_modus: " + str(clip_modus) + " --> " + str(new))
	clip_modus = new	
    
    
clip_ping_test()
		
		
		
		
@app.errorhandler(jinja2.exceptions.TemplateNotFound)
def template_not_found(e):
    return not_found(e)

@app.errorhandler(404)
def not_found(e):
    return '<strong>Page Not Found!</strong>', 404

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=True)
    