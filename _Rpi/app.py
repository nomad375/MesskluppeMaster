#!/usr/bin/env python
#============================================================================#
#   Includes
#============================================================================#
#Flask
from flask import *
import jinja2.exceptions
#Others
from subprocess import call
from random import sample
import threading
from multiprocessing import Process, Array
from messkluppe_nrf24 import *

#============================================================================#
#   globals
#============================================================================#
global g_clip_modus_pi
g_clip_modus_pi = 0

global g_buffer         # can be deleted
g_buffer = []           # can be deleted
global g_clip_files     # can be deleted
g_clip_files = 0        # can be deleted

global g_com_clip
g_com_clip = [0, 0, 0, 0, 0]
global g_com_clip_files
g_com_clip_files = []
global g_com_clip_filename
g_com_clip_filename = 0
global g_com_clip_file
g_com_clip_file = []

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

@app.route('/')
def index():
    change_clip_modus(0)
    return render_template('index.html')

@app.route('/<pagename>')
def admin(pagename):
    return render_template(pagename+'.html')

@app.route('/<path:resource>')
def serveStaticResource(resource):
	return send_from_directory('static/', resource)

@app.route('/start_logging')
def start_logging():
	change_clip_modus(20)
	return str(g_clip_modus_pi)	
	
@app.route('/stop_logging')
def stop_logging():
	change_clip_modus(0)
	return str(g_clip_modus_pi)	

@app.route('/get_globals')
def get_globals():	
    return jsonify(
		g_clip_modus_pi = g_com_clip[1],
        g_clip_modus_ad = g_com_clip[2],
        g_clip_files = len(g_com_clip_files),
        g_ping = g_com_clip[0])
          
@app.route('/file_list')
def get_file_list():
    g_com_clip_files.clear()
    change_clip_modus(30)
    return("n")
    
#@app.route('/file_list')
#def get_file_list():
#    global g_buffer
#    g_buffer.clear()
#    change_clip_modus(30)
#    return("n")
  
@app.route('/file_list_show')
def get_file_list_show():
    return render_template('file_list.html', g_buffer=g_com_clip_files)
    
@app.route('/file_show')
def get_file_show():
    return render_template('file.html', g_buffer=g_com_clip_file)
    
@app.route('/file_download/<name>')
def file_download(name):
    print ("Request: " + name)
    g_com_clip_filename = int(name)
    print ("Request now : " + str(g_com_clip_filename))
    change_clip_modus(40)
    return("n")
    
@app.route('/reset_send_clip_modus')
def reset_send_clip_modus():
    change_clip_modus(0)
    return ("n")

@app.errorhandler(jinja2.exceptions.TemplateNotFound)
def template_not_found(e):
    return not_found(e)

@app.errorhandler(404)
def not_found(e):
    return '<strong>Page Not Found!</strong>', 404


#============================================================================#
#   Change the global variable to new mode
#============================================================================#
def change_clip_modus(new):
    print("change_clip_modus: " + str(g_com_clip[1]) + " --> " + str(new))
    g_com_clip[1] = new	                                                     # Clip Modus Pi

#============================================================================#
def test():
    global p
    p.terminate()
    receivedMessage = [0, 0, 0, 0, 0, 0, 0, 0,]
    RcvMsg = [0, 0, 0, 0, 0, 0, 0, 0,]
    SndMsg=[0, 0, 0, 0, 0, 0, 0, 0,]
    while 1:
         if (radio.available()):
            size = radio.getDynamicPayloadSize()
            if (size == 32):
                #correct size of the payload
                radio.read(receivedMessage, size)
                RcvMsg = translate_from_radio(receivedMessage, size)
                print (str(RcvMsg))
                
                SndMsg = [1030, RcvMsg[1], int(time.time()),0,0,0,0,0]
                SndMsg = translate_to_radio(SndMsg)
                radio.writeAckPayload(1, SndMsg, len(SndMsg))
                
            if (RcvMsg[3] == 44):
                break

  
#============================================================================#
def com_clip ():#(g_com_clip):   
    receivedMessage = [0, 0, 0, 0, 0, 0, 0, 0,]
    RcvMsg = [0, 0, 0, 0, 0, 0, 0, 0,]
    SndMsg=[0, 0, 0, 0, 0, 0, 0, 0,]
    last_timestamp = 0
    #global g_com_clip
    
    while 1:
        #print("running")
        if (radio.available()):
            #print("Radio Available")
            size = radio.getDynamicPayloadSize()
            if (size == 32):
                #print("Size == 32")
                radio.read(receivedMessage, size)
                RcvMsg = translate_from_radio(receivedMessage, size, True)
                Rcv_idTask = idTask(RcvMsg[0])
                
                if (0 < Rcv_idTask[0] < 100 ):                               # ID must be smaller then 100
                    g_com_clip[2] = Rcv_idTask[1]                            # Save Arduino Mode 
                    
                    #---------- Ping Mode -----------------------------------#
                    if (g_com_clip[1] == 0):                                 # Clip Modus Pi 
                        ping = RcvMsg[1] - last_timestamp                    # Calculate the Ping
                        last_timestamp = RcvMsg[1]                           # Save the last Timestamp 
                        
                        if (0 < ping < 100):                                 # Good Ping 
                            SndMsg = [RcvMsg[0], RcvMsg[1], int(time.time()),0,0,0,0,0]
                            SndMsg = translate_to_radio(SndMsg)
                            radio.writeAckPayload(1, SndMsg, len(SndMsg))
                            g_com_clip[0] = ping                             # Clip ping 
                            
                    #---------- Start Logging -------------------------------#
                    if (g_com_clip[1] == 20):                                # Clip Modus Pi 
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])   # Clip Modus PI
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))                                 
                   
                    #---------- Start Get File List -------------------------#
                    if (g_com_clip[1] == 30):
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        change_clip_modus(0)
                                    
                    #---------- Collect File List ---------------------------#     
                    if (Rcv_idTask[1] == 30):
                        g_com_clip_files.append(RcvMsg) 
                            
                    #---------- Start Get File List -------------------------#
                    if (g_com_clip[1] == 40):
                        print("40: " + str(g_com_clip_filename))
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),1530200409 ,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        change_clip_modus(0)
                        
                    #---------- Collect File List ---------------------------#     
                    if (Rcv_idTask[1] == 40):
                        g_com_clip_file.append(RcvMsg) 
                            
            else:
                g_com_clip[0] = -1
                                    
                                    
@app.before_first_request
def activate_job():		
    t = threading.Thread(target=com_clip)
    t.start()
#============================================================================#
#   start Flask Server
#============================================================================# 		
if __name__ == '__main__':
        
    print("Im here")
    print(str(threading.active_count()))
    print(str(threading.enumerate()))
    
        
    app.run(host='0.0.0.0', port=5001, debug=True)
        #global g_com_clip2
        #g_com_clip = Array('i', 5)
        #if 'g_com_clip_2' in globals():
        #   print ("I Have!!!")
        #p = Process(target=com_clip, args=(g_com_clip,))
        #p.start()
       # p.join()
        		
#============================================================================# 	


    