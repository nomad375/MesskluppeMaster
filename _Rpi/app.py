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
g_com_clip_live = [0, 0, 0, 0, 0, 0]
global g_com_clip_files
g_com_clip_files = []
global g_com_clip_filename
g_com_clip_filename = 0
global g_com_clip_file
g_com_clip_file = []
global g_path
g_path = "/home/pi/Flask_Messkluppe/static/_csv/"

global g_clip
g_clip = {}
g_clip['mode_pi'] = 0
g_clip['dl_started'] = False
g_clip['dl_finished'] = False
g_clip['dl_successfull'] = False
g_clip['online_files'] = []

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
radio.setChannel(111)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_HIGH)
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

@app.route('/get_threads')
def get_threads():
    print (threading.enumerate())
    return " "
    
@app.route('/start_logging')
def start_logging():
	change_clip_modus(20)
	return str(" ")	
	
@app.route('/stop_logging')
def stop_logging():
	change_clip_modus(0)
	return str(" ")	

@app.route('/get_globals')
def get_globals():
    g_clip['dl_file_len'] =  len(g_com_clip_file)
    return jsonify(g_clip)
        
@app.route('/file_download/<name>/<lines>')
def file_download(name, lines):
    g_clip['dl_finished'] = False
    g_clip['dl_started'] = False
    g_com_clip_file.clear()
    g_clip['dl_filename'] = int(name)
    g_clip['dl_from'] = 1
    g_clip['dl_until'] = int(lines)
    g_clip['dl_lines'] = int(lines)   

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
    files = []
    for filename in os.listdir(g_path):
        file = os.stat(g_path+filename)
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
    with open(g_path+name) as file:
        csvReader = csv.reader(file)
        for row in csvReader:
            data.append(row)

    return render_template('m_local_file.html', file=name, data=data)
#============================================================================#
#   Displays all files which are on the Arduino
#   2018-08-10
#   2018-09-10  filenames is in global so render the template and append the 
#               files after reciving all
#============================================================================#
@app.route('/online_files')
def online_files():
    g_clip['online_files'].clear()
    change_clip_modus(30)
     
    return render_template('m_online_files.html')
    
#============================================================================#
#   Change the global variable to new mode
#============================================================================#
def change_clip_modus(new):
    print("change_clip_modus: " + str(g_clip['mode_pi']) + " --> " + str(new))
    g_clip['mode_pi'] = new	                                                     # Clip Modus Pi
#============================================================================#
#============================================================================#
#   Create CSV
#============================================================================#
def create_csv(name, data):
    f = open(g_path+str(name)+'.csv', 'w')
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
        filepath = g_path+str(filelist[x][4])+".csv"
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
    timeout = time.time()
    lastMsg = timeout


    
    while 1:
        #print("running")
        g_clip['com_noMsg'] = round(time.time()-lastMsg, 0)
        if (radio.available()):
            
            size = radio.getDynamicPayloadSize()
            #print("Radio Available size:" + str(size))
            
             
            if (size == 32):
                lastMsg = time.time()
                radio.read(receivedMessage, size)
                RcvMsg = translate_from_radio(receivedMessage, size, True)
                
                Rcv_idTask = idTask(RcvMsg[0])
                timeout = time.time()
                #print (Rcv_idTask)
                if (0 < Rcv_idTask[0] < 100 ):                               # ID must be smaller then 100
                    g_clip['mode_ad'] = Rcv_idTask[1]                            # Save Arduino Mode 
                    
                    
                   
                            
                    #---------- Start Logging -------------------------------#
                    if (g_clip['mode_pi'] == 20):                                # Clip Modus Pi 
                        print ("Start Logging")
                        newIdTask = idTask([Rcv_idTask[0], g_clip['mode_pi']])   # Clip Modus PI
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg, False)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))                                 
                   
                    #---------- Finished File List --------------------------#     
                    if (Rcv_idTask[1] == 31):                                # Clip Modus Arduino
                        print ("Finished get file list")
                        g_clip['dl_finished'] = True                       
                   
                    #---------- Collect File List ---------------------------#     
                    if (Rcv_idTask[1] == 30):                                # Clip Modus Arduino
                        g_clip['online_files'].append(RcvMsg)
                        change_clip_modus(0)
                    
                    #---------- Start Get File List -------------------------#
                    if (g_clip['mode_pi'] == 30):                                # Clip Modus Pi
                        print ("Start get file list")
                        newIdTask = idTask([Rcv_idTask[0], g_clip['mode_pi']])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg, False)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        
                        g_clip['dl_finished'] = False                                # download_finished
                        g_clip['dl_successfull'] = False                                # download_successfull
                        
                    #---------- Finished Collect File -----------------------#  
                    if (Rcv_idTask[1] == 41):                                # Clip Modus Arduino
                        print ("Finished get file")                          #
                        g_clip['dl_finished'] = True                                 # download_finished
                                       
                    #---------- Collect File --------------------------------#     
                    if (Rcv_idTask[1] == 40):                                # Clip Modus Arduino
                        g_clip['dl_started'] = True                                 # download_started
                        g_clip['dl_finished'] = False                                # download_finished
                        g_clip['dl_successfull'] = False                                # download_successfull
                        g_com_clip_file.append(RcvMsg)
                        change_clip_modus(0)
                        continue
                        
                    
                            
                    #---------- Start Get File ------------------------------#
                    if (g_clip['mode_pi'] == 40):                                # Clip Modus Pi
                        print ("Start get file")
                        newIdTask = idTask([Rcv_idTask[0], g_clip['mode_pi']])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),g_clip['dl_filename'] ,g_clip['dl_from'], g_clip['dl_until'],0,0]
                        SndMsg = translate_to_radio(SndMsg, False)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        
                        g_clip['dl_finished'] = False                                # download_finished
                        g_clip['dl_successfull'] = False                                # download_successfull
                        
                  
                        
                    
                    #---------- Check if download is finished ---------------#    
                    #print("download_started: " + str(download_started) + " download_finished: " + str(download_finished) + " max lines " + str(g_com_clip[6]) + " downloaded: " + str(len(g_com_clip_file)))
                    if (g_clip['dl_started'] == True and g_clip['dl_finished'] == True):    # download_started & download_finished
                        print ("Check download status") 
                        if (g_clip['dl_lines'] == len(g_com_clip_file)):          # Check if we get all lines
                            g_clip['dl_successfull'] = True                             # download_successfull  
                            create_csv(g_clip['dl_filename'], g_com_clip_file)
                        else:
                            print ("Download not Sucessfull")
                            g_clip['dl_successfull'] = False                            # download_successfull
         
                        # to be deleted!
                        create_csv(g_clip['dl_filename'], g_com_clip_file)  

                        g_clip['dl_finished'] = False                                # download_finished
                        g_clip['dl_started'] = False                                # download_started
                        
                    #---------- Live Data -----------------------------------#
                    if (g_clip['mode_pi'] == 60):                                # Clip Modus Pi    
                        newIdTask = idTask([Rcv_idTask[0], g_clip['mode_pi']])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg, False)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        
                    if (Rcv_idTask[1] == 60): 
                       g_clip['live_x'] = RcvMsg[4]
                       g_clip['live_y'] = RcvMsg[5]
                       g_clip['live_z'] = RcvMsg[6]
                       g_clip['live_board'] = RcvMsg[7]%10000
                       g_clip['live_clip'] = (RcvMsg[7]-g_clip['live_board'])/10000
                       g_clip['live_volt'] = RcvMsg[3]
                             
                     #---------- Ping Mode ----------------------------------#
                    if (g_clip['mode_pi'] == 0):                                 # Clip Modus Pi 
                        ping = RcvMsg[1] - last_timestamp                    # Calculate the Ping
                        last_timestamp = RcvMsg[1]                           # Save the last Timestamp 
                        
                        if (0 < ping < 100):                                 # Good Ping
                            SndMsg = [RcvMsg[0], RcvMsg[1], int(time.time()),0,0,0,0,0]
                            SndMsg = translate_to_radio(SndMsg, False)
                            radio.writeAckPayload(1, SndMsg, len(SndMsg))
                            g_clip['com_ping'] = RcvMsg[2]                             # Clip ping
                            g_clip['com_success'] = RcvMsg[3]
                     
                        
            else:
                #---------- Radio crashed! ----------------------------------#
                radio.stopListening()
                radio.startListening()
                g_clip['com_ping'] = -1
                
        #---------- Connection Timeout --------------------------------------#
        elif (time.time() - timeout > 2):                                    # > 2 sec
            g_clip['com_ping'] = -1                                               # Ping
            g_clip['com_success'] = 0 
            g_clip['live_x'] = 0                                           # Live X
            g_clip['live_y'] = 0                                           # Live Y
            g_clip['live_z'] = 0                                           # Live Z
            g_clip['live_board'] = 0                                           # Live Bat
            g_clip['live_clip'] = 0 
            g_clip['live_volt'] = 0 
            
            print("timeout")
        #---------- No Radio ------------------------------------------------#
    
    
        
                                    
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


    