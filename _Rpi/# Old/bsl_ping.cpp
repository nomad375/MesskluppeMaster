#include <cstdlib>
#include <iostream>
#include "RF24/RF24.h"
#include <fstream> 
#include <time.h>
#include <stdio.h>

using namespace std;
 
// spi device, spi speed, ce gpio pin
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.



void setup(void)
{
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setChannel(55);
  
  //radio.openWritingPipe(pipes[0]);			//Where we send data out
  radio.openReadingPipe(1,pipes[1]);		// Where we receive data back
  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_1MBPS);

  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.enableAckPayload();
     
  radio.setRetries(2,15);  					//(2,15) 
  radio.setCRCLength(RF24_CRC_8);			// Use 8-bit CRC for performance
  
  radio.startListening();
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  
  
  radio.powerUp();                        //Power up the radio

//////////////
//radio.writeAckPayload(1, &AckPayload, sizeof(AckPayload)); // load the payload for the FIRST time


}


uint32_t ping (uint32_t mode=0) {
	// Prepare all variables
	uint32_t SendMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t timestamp = millis();
	uint32_t ping = 0;

	
	time_t now;
	time(&now);
	//printf(str(now));
	//struct tm * timeinfo;
	//time_t timeinfo;
	//timeinfo = localtime (&now);
	//printf ("Current local time and date: %s", timeinfo);


	
	radio.powerUp();
	radio.startListening();
	while (millis()-timestamp < 500){

		
		if (radio.available()){
			uint32_t len = radio.getDynamicPayloadSize();
			
			radio.read(&RcvMsg, len);
			cout << "Payload we got: [0]" << RcvMsg[0] << " [1]" << RcvMsg[1] << " [2]" << RcvMsg[2] << " [3]" << RcvMsg[3] << " [4]" << RcvMsg[4] << " [5]" << RcvMsg[5] << " [6]" << RcvMsg[6] << " [7]" << RcvMsg[7] << "\n";
			cout << "Payload we snd: [0]" << SendMsg[0] << " [1]" << SendMsg[1] << " [2]" << SendMsg[2] << " [3]" << SendMsg[3] << " [4]" << SendMsg[4] << " [5]" << SendMsg[5] << " [6]" << SendMsg[6] << " [7]" << SendMsg[7] << "\n";

			
			ping = RcvMsg[1] - SendMsg[1];
			//cout << "Ping: " << ping << "\n";
			
			// Prepare the anser
			SendMsg[0] = RcvMsg[0]; // Send back the Clip ID
			SendMsg[1] = RcvMsg[1]; // Send back the timestamp
			SendMsg[2] = mode;		// Send the mode
			SendMsg[3] = now;
			radio.writeAckPayload(1, &SendMsg, sizeof(SendMsg));
			
			
			if (ping > 0 && ping < 100000){
				return ping;
			}
		}

	}
	return 0;
}

 
 
int main(int argc, char* argv[]) {
	setup();

	int parameter = atoi(argv[1]);
	uint32_t time = ping(parameter);
	
	return time;

		
}
