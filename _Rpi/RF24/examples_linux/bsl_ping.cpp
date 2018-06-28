#include <cstdlib>
#include <iostream>
#include "../RF24.h"
#include <fstream> 

using namespace std;
 
// spi device, spi speed, ce gpio pin
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.



void setup(void)
{
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setChannel(77);
  
  //radio.openWritingPipe(pipes[0]);			//Where we send data out
  radio.openReadingPipe(1,pipes[1]);		// Where we receive data back
  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_1MBPS);

  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.enableAckPayload();
     
  radio.setRetries(2,15);  					//(2,15) 
  radio.setCRCLength(RF24_CRC_8);			// Use 8-bit CRC for performance
  
  radio.startListening();
  //radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  
  
  radio.powerUp();                        //Power up the radio

//////////////
//radio.writeAckPayload(1, &AckPayload, sizeof(AckPayload)); // load the payload for the FIRST time


}

int ping () {
	// Prepare all variables
	uint32_t SendMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t timestamp = millis();
	int ping = 0;
	
	//radio.powerUp();
	//radio.startListening();
	while (millis()-timestamp < 20000){
		delay (15);
		if (radio.available()){
			uint32_t len = radio.getDynamicPayloadSize();
			
			radio.read(&RcvMsg, len);
			printf(" Paylod we got: %u, %u, %u, %u, %u, %u, %u, %u, %u \n\r", RcvMsg[0], RcvMsg[1], RcvMsg[2], RcvMsg[3], RcvMsg[4], RcvMsg[5], RcvMsg[6],RcvMsg[7], len);
			printf(" Paylod we send: %u, %u, %u, %u, %u, %u, %u, %u, %u \n\r", SendMsg[0], SendMsg[1], SendMsg[2], SendMsg[3], SendMsg[4], SendMsg[5], SendMsg[6],SendMsg[7], len);
			ping = RcvMsg[1] - SendMsg[1];
			
			
			// Prepare the anser
			SendMsg[0] = RcvMsg[0]; // Send back the Clip ID
			SendMsg[1] = RcvMsg[1]; // Send back the timestamp
			radio.writeAckPayload(1, &SendMsg, sizeof(SendMsg));
			
			return ping;
		}
		else {
			return 9999;
		}
	}
	return 2;
}
 
 
int main(int argc, char** argv) 
{

cout << "Ping\n";

setup();
//while (1) {
	silvo=ping();
//}


  
//cout << "Please check values of output file" << endl;


    return silvo;
}
