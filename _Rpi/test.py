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
def translate_from_radio(msg, size, debug=False):
    translated_msg=[]
    for i in range (0, size, 4):
        translated_msg.append(int.from_bytes([msg[i+3], msg[i+2], msg[i+1], msg[i]], byteorder='big')) 
        
    if (debug):
        print("Translate FROM Radio: " + str(msg) + " --> " + str(translated_msg))
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
#============================================================================#
#   Read the payload until new mode
#============================================================================#
def read_to_buffer (task, timeout):
    print ("restart!!!")
    got_idTask = [0, task]
    start_time = time.time()
    buffer = []
    receivedMessage = []
    while (time.time() - start_time < timeout): #task == got_idTask[1] and
        
        #print ("While!!! Task: " + str(got_idTask[1]) + "time: " + str(time.time() - start_time))
        if (radio.available()):
            size = radio.getDynamicPayloadSize()
            if (size == 32):
                
                radio.read(receivedMessage, size)
                RcvMsg = translate_from_radio(receivedMessage, size, True)
                
                got_idTask = idTask(RcvMsg[0])
                
                newIdTask = idTask([got_idTask[0], g_clip_modus_pi])
                SndMsg = [newIdTask, RcvMsg[1], int(time.time()),g_clip_filename,0,0,0,0]

                SndMsg = translate_to_radio(SndMsg)
                            
                radio.writeAckPayload(1, SndMsg, len(SndMsg))
                
                
                
                if (0 < got_idTask[0] < 100 ):  
                    buffer.append(RcvMsg)
    #print(str(buffer))
    return buffer
    
receivedMessage = []


 def com_clip ():#(g_com_clip):   
    receivedMessage = [0, 0, 0, 0, 0, 0, 0, 0,]
    RcvMsg = [0, 0, 0, 0, 0, 0, 0, 0,]
    SndMsg=[0, 0, 0, 0, 0, 0, 0, 0,]
    last_timestamp = 0
    download_started = False
    download_finished = False
    #global g_com_clip
    
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
                    
                    #---------- Ping Mode -----------------------------------#
                    if (g_com_clip[1] == 0):                                 # Clip Modus Pi 
                        ping = RcvMsg[1] - last_timestamp                    # Calculate the Ping
                        last_timestamp = RcvMsg[1]                           # Save the last Timestamp 
                        
                        if (0 < ping < 100):                                 # Good Ping 
                            SndMsg = [RcvMsg[0], RcvMsg[1], int(time.time()),0,0,0,0,0]
                            SndMsg = translate_to_radio(SndMsg)
                            radio.writeAckPayload(1, SndMsg, len(SndMsg))
                            g_com_clip[0] = ping                             # Clip ping
                            download_finished = True
                            
                    #---------- Start Logging -------------------------------#
                    if (g_com_clip[1] == 20):                                # Clip Modus Pi 
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])   # Clip Modus PI
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))                                 
                   
                    #---------- Start Get File List -------------------------#
                    if (g_com_clip[1] == 30):                                # Clip Modus Pi
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),0,0,0,0,0]
                        SndMsg = translate_to_radio(SndMsg)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        change_clip_modus(0)
                                    
                    #---------- Collect File List ---------------------------#     
                    if (Rcv_idTask[1] == 30):                                # Clip Modus Arduino
                        g_com_clip_files.append(RcvMsg) 
                            
                    #---------- Start Get File ------------------------------#
                    if (g_com_clip[1] == 40):                                # Clip Modus Pi
                        newIdTask = idTask([Rcv_idTask[0], g_com_clip[1]])
                        SndMsg = [newIdTask, RcvMsg[1], int(time.time()),g_com_clip[3] ,g_com_clip[4], g_com_clip[5],0,0]
                        SndMsg = translate_to_radio(SndMsg)
                        radio.writeAckPayload(1, SndMsg, len(SndMsg))
                        change_clip_modus(0)
                        download_finished = False
                        
                    #---------- Collect File --------------------------------#     
                    if (Rcv_idTask[1] == 40):                                # Clip Modus Arduino
                        download_started = True
                        download_finished = False
                        g_com_clip_file.append(RcvMsg) 
                        
                    #print("download_started: " + str(download_started) + " download_finished: " + str(download_finished) + " max lines " + str(g_com_clip[6]) + " downloaded: " + str(len(g_com_clip_file)))
                    if (download_started == True and download_finished == True):
                        if (g_com_clip[6] == len(g_com_clip_file)):              # Check if we get all lines
                            print("All lines downloaded")
                        else:
                            print("Something missing")

                        download_finished = False
                        download_started = False
                    
            else:
                g_com_clip[0] = -1
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            