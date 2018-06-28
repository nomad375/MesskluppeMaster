import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev




GPIO.setmode(GPIO.BCM)

pipes = [0xAB, 0xCD, 0xAB, 0xCD, 0x71]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 22)

radio.setPayloadSize(32)
radio.setChannel(77)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)

radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()

radio.openReadingPipe(1,pipes)
radio.printDetails()
radio.startListening()






while(1):
    # ackPL = [1]
    while not radio.available(0):
        time.sleep(1 / 100)
    receivedMessage = []
    radio.read(receivedMessage, radio.getDynamicPayloadSize())
    

    
    print("Received: {}".format(receivedMessage))
    print(int.from_bytes([receivedMessage[3], receivedMessage[2], receivedMessage[1], receivedMessage[0]], byteorder='big'))
    
    
    
    

    #print("Translating the receivedMessage into unicode characters")
    #string = "0x"+str(hex(receivedMessage[0])[-2:])#+str(hex(receivedMessage[1]))+str(hex(receivedMessage[2])+str(hex(receivedMessage[3])
   # print(string)
    #print(int(0x222000,8))
    

    
    
  
 