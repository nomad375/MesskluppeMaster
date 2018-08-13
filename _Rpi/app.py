#!/usr/bin/env python
#============================================================================#
#   Includes
#============================================================================#
#Flask
from flask import *
import jinja2.exceptions
#Others
import threading
from messkluppe_nrf24 import *
import csv
import os.path
import time
import datetime

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
g_com_clip = [0, 0, 0, 0, 0,0 ,0 ,False ,False ,False ,0]
global g_com_clip_live
g_com_clip_live = [0, 0, 0, 0]
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
    change_clip_modus(0)
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
        g_clip_file = len(g_com_clip_file),
        g_ping = g_com_clip[0],
        g_download_finished = g_com_clip[7],
        g_download_started = g_com_clip[8],
        g_download_successfull = g_com_clip[9],
        g_live_x = g_com_clip_live[0],
        g_live_y = g_com_clip_live[1],
        g_live_z = g_com_clip_live[2],
        g_live_w = g_com_clip_live[3])
          
@app.route('/file_list')
def get_file_list():
    g_com_clip_files.clear()
    change_clip_modus(30)
    return("n")
  
@app.route('/file_list_show')
def get_file_list_show():
    global g_com_clip_files 
   
    g_com_clip_files = check_filelist(g_com_clip_files)
    return render_template('file_list.html', g_buffer=g_com_clip_files)
        
@app.route('/file_download/<name>/<lines>')
def file_download(name, lines):
    g_com_clip[7] = False
    g_com_clip[8] = False
    g_com_clip_file.clear()
    g_com_clip[3] = int(name)                   # Filename
    g_com_clip[4] = 1                           # Download from     (1)
    g_com_clip[5] = int(lines)                  # Download until    (last line)
    g_com_clip[6] = int(lines)                  # File Lines
    change_clip_modus(40)
    
    return render_template('file.html', g_buffer=g_com_clip_file)

@app.route('/live_data')
def live_data():    
       change_clip_modus(60)
       return render_template('live_data.html')
       
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
#   Displays all csv files stored on the raspberry
#   #2018-08-10
#============================================================================#
@app.route('/local_files')
def local_files():  
    p = "static/_csv/"
    files = []
    for filename in os.listdir(p):
        file = os.stat(p+filename)
        size = file.st_size
        date = datetime.datetime.fromtimestamp(file.st_mtime).strftime('%Y-%m-%d %H:%M:%S')
        files.append([filename, size, date])
    
    print(files)
    return render_template('m_local_files.html', files=files)
#============================================================================#
#   Displays one csv as graph
#   2018-08-10
#============================================================================#
@app.route('/local_file/<name>')
def local_file(name):
    data = []
    with open('static/_csv/'+name) as file:
        csvReader = csv.reader(file)
        for row in csvReader:
            data.append(row)

    return render_template('m_local_file.html', file=name, data=data)
#============================================================================#
#   Displays all files which are on the Arduino
#   2018-08-10
#============================================================================#
@app.route('/online_files')
def online_files():
    change_clip_modus(30)
     
    return render_template('m_online_files.html', files=name)
    
#============================================================================#
#   Change the global variable to new mode
#============================================================================#
def change_clip_modus(new):
    print("change_clip_modus: " + str(g_com_clip[1]) + " --> " + str(new))
    g_com_clip[1] = new	                                                     # Clip Modus Pi
#============================================================================#
#============================================================================#
#   Create CSV
#============================================================================#
def create_csv(name, data):
    f = open('static/_csv/'+str(name)+'.csv', 'w')
    with f:
        writer = csv.writer(f)
        writer.writerows(data)    

#============================================================================#
#============================================================================#
#   Check Filelist
#============================================================================#
def check_filelist(filelist):
    for x in range(0, len(filelist)):
        #Check if file exist already and is complete
        filepath = "static/_csv/"+str(filelist[x][4])+".csv"
        #filelist[1] will hold all propertys
        # Check if file exists and is completly downloaded
        if os.path.isfile(filepath) and len(open(filepath).readlines()) == filelist[x][6]:  
            filelist[x][1] = {'complete': True}
        else:
            filelist[x][1] = {'complete': False}
        print(filelist[x])
    
    return filelist


#============================================================================#
def com_clip ():  
    receivedMessage = [0, 0, 0, 0, 0, 0, 0, 0,]
    RcvMsg = [0, 0, 0, 0, 0, 0, 0, 0,]
    SndMsg=[0, 0, 0, 0, 0, 0, 0, 0,]
    last_timestamp = 0


    
    while 1:
        #print("running")
        if (radio.available()):
            #print("Radio Available")
            size = radio.getDynamicPayloadSize()
            if (size == 32):
                #print("Size == 32")
                radio.read(receivedMessage, size)
                RcvMsg = translate_from_radio(receivedMessage, size, False)
                Rcv_idTask = idTask(RcvMsg[0])
                
                if (0 < Rcv_idTask[0] < 100 ):                               # ID must be smaller then 100
                    g_com_clip[2] = Rcv_idTask[1]                            # Save Arduino Mode 
                    
                   
                            
                    #---------- Start Logging -------------------------------#
                    if (g_com_clip[1] == 20):                                # Clip Modus Pi 
                        print ("Start Logging")
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])   # Clip Modus PI
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg, False)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))                                 
                   
                    #---------- Start Get File List -------------------------#
                    if (g_com_clip[1] == 30):                                # Clip Modus Pi
                        print ("Start get file list")
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg, False)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        change_clip_modus(0)
                                    
                    #---------- Collect File List ---------------------------#     
                    if (Rcv_idTask[1] == 30):                                # Clip Modus Arduino
                        g_com_clip_files.append(RcvMsg)
                    #---------- Finished File List --------------------------#     
                    if (Rcv_idTask[1] == 31):                                # Clip Modus Arduino
                        g_com_clip[7] = True                       
                            
                    #---------- Start Get File ------------------------------#
                    if (g_com_clip[1] == 40):                                # Clip Modus Pi
                        print ("start get file")
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),g_com_clip[3] ,g_com_clip[4], g_com_clip[5],0,0]
                        SndMsg = translate_to_radio(SndMsg, False)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        change_clip_modus(0)
                        g_com_clip[7] = False                                # download_finished
                        g_com_clip[9] = False                                # download_successfull
                        
                    #---------- Collect File --------------------------------#     
                    if (Rcv_idTask[1] == 40):                                # Clip Modus Arduino
                        g_com_clip[8] = True                                 # download_started
                        g_com_clip[7] = False                                # download_finished
                        g_com_clip[9] = False                                # download_successfull
                        g_com_clip_file.append(RcvMsg)
                        continue
                    if (Rcv_idTask[1] == 41):                                # Clip Modus Arduino
                        g_com_clip[7] = True                                 # download_finished
                        
                    #print("download_started: " + str(download_started) + " download_finished: " + str(download_finished) + " max lines " + str(g_com_clip[6]) + " downloaded: " + str(len(g_com_clip_file)))
                    if (g_com_clip[8] == True and g_com_clip[7] == True):    # download_started & download_finished
                        if (g_com_clip[6] == len(g_com_clip_file)):          # Check if we get all lines
                            g_com_clip[9] = True                             # download_successfull  
                            create_csv(g_com_clip[3], g_com_clip_file)
                        else:
                            g_com_clip[9] = False                            # download_successfull
                            
                            y = 1
                            yy = 0
                            for x in g_com_clip_file:
                                
                                while (x[3] != y):
                                    print("x: " + str(x) + " y: " + str(y))
                                    g_com_clip_file.insert(y-1, ["Silvo", 0,0,y+yy])
                                    yy += 1
                                    time.sleep(1)
                                else: 
                                    y += 1
                                    yy = 0
                                    
                        create_csv(g_com_clip[3], g_com_clip_file)  

                        g_com_clip[7] = False                                # download_finished
                        g_com_clip[8] = False                                # download_started
                        
                    #---------- Live Data -----------------------------------#
                    if (g_com_clip[1] == 60):                                # Clip Modus Pi    
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg, False)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        
                    if (Rcv_idTask[1] == 60): 
                       g_com_clip_live[0] = RcvMsg[4]
                       g_com_clip_live[1] = RcvMsg[5]
                       g_com_clip_live[2] = RcvMsg[6]
                       g_com_clip_live[3] = RcvMsg[7]
                             
                     #---------- Ping Mode -----------------------------------#
                    if (g_com_clip[1] == 0):                                 # Clip Modus Pi 
                        ping = RcvMsg[1] - last_timestamp                    # Calculate the Ping
                        last_timestamp = RcvMsg[1]                           # Save the last Timestamp 
                        
                        if (0 < ping < 100):                                 # Good Ping
                            SndMsg = [RcvMsg[0], RcvMsg[1], int(time.time()),0,0,0,0,0]
                            SndMsg = translate_to_radio(SndMsg, False)
                            radio.writeAckPayload(1, SndMsg, len(SndMsg))
                            g_com_clip[0] = ping                             # Clip ping
                    
                        
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
    app.run(host='0.0.0.0', port=5001, debug=True)     		
#============================================================================# 	


    