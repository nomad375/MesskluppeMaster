#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Example program to receive packets from the radio link
#

import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
from lib_nrf24 import NRF24
import time
import spidev



#pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]
#pipes = [0xABCDABCD71, 0x544d52687C ]

pipes = [[0xAB, 0xCD, 0xAB, 0xCD, 0x71], [0x54, 0x4D, 0x52, 0x68, 0x7C]]


radio2 = NRF24(GPIO, spidev.SpiDev())
radio2.begin(0, 22)

radio2.setRetries(15,15)

radio2.setPayloadSize(32)
radio2.setChannel(77)
radio2.setDataRate(NRF24.BR_1MBPS)
radio2.setPALevel(NRF24.PA_MIN)

radio2.setAutoAck(True)
radio2.enableDynamicPayloads()
radio2.enableAckPayload()

radio2.openWritingPipe(pipes[0])
radio2.openReadingPipe(1, pipes[1])

radio2.startListening()
radio2.stopListening()

radio2.printDetails()

radio2.startListening()

c=1
time.sleep(5)


while True:
    akpl_buf = [c,1, 2, 3,4,5,6,7,8,9,0,1, 2, 3,4,5,6,7,8]
    pipe = [0]
    while not radio2.available(pipe):
        time.sleep(1)
        print("No Radio")

    recv_buffer = []
    radio2.read(recv_buffer, radio2.getDynamicPayloadSize())
    print ("Received:") ,
    print (recv_buffer)
    c = c + 1
    if (c&1) == 0:
        radio2.writeAckPayload(1, akpl_buf, len(akpl_buf))
        print ("Loaded payload reply:"),
        print (akpl_buf)
    else:
        print ("(No return payload)")
