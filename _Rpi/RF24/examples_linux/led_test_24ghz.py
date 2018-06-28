#!/usr/bin/env python
#from __future__ import print_function
import time
from RF24 import *
import RPi.GPIO as GPIO
 
button_pin  = 16        # GPIO number
codeOn      = "Turn LED on"
codeOff     = "Turn LED off"
 
# RPi Alternate, with SPIDEV - Note: Edit RF24/arch/BBB/spi.cpp and  set 'this->device = "/dev/spidev0.0";;' or as listed in /dev
radio = RF24(22, 0);
 
pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]
payload_size = 32
millis = lambda: int(round(time.time() * 1000))
 
radio.begin()
radio.enableDynamicPayloads()
radio.setRetries(5,15)
 
radio.openWritingPipe(pipes[0])
radio.openReadingPipe(1,pipes[1])
 
def sendCode(code):
    # The payload will always be the same, what will change is how much of it we send.
 
    # First, stop listening so we can talk.
    radio.stopListening()
 
    # Take the time, and send it.  This will block until complete
    radio.write(code[:payload_size])
 
    # Now, continue listening
    radio.startListening()
 
    # Wait here until we get a response, or timeout
    started_waiting_at = millis()
    timeout = False
    while (not radio.available()) and (not timeout):
        if (millis() - started_waiting_at) > 500:
            timeout = True
 
    # Describe the results
    if timeout:
        return False
    else:
        # Grab the response, compare, and send to debugging spew
        len = radio.getDynamicPayloadSize()
        receive_payload = radio.read(len)
        return True
 
 
if __name__ == "__main__":
 
    led_status = False
 
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(button_pin, GPIO.IN)
 
    while True:
        if GPIO.input(button_pin) == GPIO.HIGH:
            code = codeOff if led_status else codeOn
            while sendCode(code) == False:
                print("Sending failed... Try again")
                time.sleep(0.1)
            led_status = not led_status
            print("LED Status on Arduino: %s" % ("ON" if led_status else "OFF") )
            # cooldown
            time.sleep(0.3)
            
        time.sleep(0.1)
