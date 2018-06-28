#!/usr/bin/env python

#
# Example using Dynamic Payloads
# 
#  This is an example of how to use payloads of a varying (dynamic) size.
# 

from __future__ import print_function
import time
from RF24 import *
import RPi.GPIO as GPIO

irq_gpio_pin = None

########### USER CONFIGURATION ###########
# See https://github.com/TMRh20/RF24/blob/master/RPi/pyRF24/readme.md

# CE Pin, CSN Pin, SPI Speed

# Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 8Mhz
radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ)

#RPi B
# Setup for GPIO 15 CE and CE1 CSN with SPI Speed @ 8Mhz
#radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ)

#RPi B+
# Setup for GPIO 22 CE and CE0 CSN for RPi B+ with SPI Speed @ 8Mhz
#radio = RF24(RPI_BPLUS_GPIO_J8_15, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ)

# RPi Alternate, with SPIDEV - Note: Edit RF24/arch/BBB/spi.cpp and  set 'this->device = "/dev/spidev0.0";;' or as listed in /dev
#radio = RF24(22, 0);


# Setup for connected IRQ pin, GPIO 24 on RPi B+; uncomment to activate
#irq_gpio_pin = RPI_BPLUS_GPIO_J8_18
#irq_gpio_pin = 24

##########################################
pipes = [0xABCDABCD71, 0x544d52687C]

radio.begin()
radio.enableDynamicPayloads()
radio.setChannel(77)

radio.openReadingPipe(1, pipes[1])
radio.setPALevel(RF24_PA_MAX)
radio.setDataRate(RF24_1MBPS)
radio.setAutoAck(1)
radio.enableAckPayload()
radio.setRetries(2,15) 
radio.setCRCLength(RF24_CRC_8);
radio.openWritingPipe(pipes[0])
radio.openReadingPipe(1,pipes[1])

  
#radio.startListening()
radio.printDetails()  
#radio.powerUp()

#AckPayload[4] = {};

#radio.writeAckPayload(1, AckPayload, sizeof(AckPayload))
data = []


#Wait for data
pipe =[0]
while not radio.available(pipes[0]):
	time.sleep(10000/1000000.0)

#Receive Data
recv_buffer = []
radio.read(recv_buffer)

#Print the buffer
print (recv_buffer)



















