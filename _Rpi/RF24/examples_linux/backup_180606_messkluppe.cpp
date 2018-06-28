#include <cstdlib>
#include <iostream>
#include "../RF24.h"
#include <fstream> 

using namespace std;
 
// spi device, spi speed, ce gpio pin
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
//RF24 radio(22,0);
 
uint32_t counter = 0;
uint32_t rxTimer =0;

time_t rawtime;

uint32_t AckPayload[4] = {UINT32_MAX}; 
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.



void setup(void)
{


  radio.begin();
  radio.enableDynamicPayloads();
  radio.setChannel(125);
  
  //radio.openWritingPipe(pipes[0]);			//Where we send data out
  radio.openReadingPipe(1,pipes[1]);		// Where we receive data back
  
  radio.setPALevel (RF24_PA_MAX);
  //radio.setDataRate (RF24_2MBPS);
  radio.setDataRate (RF24_1MBPS);
  //radio.setDataRate (RF24_250KBPS);
 
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  
  radio.enableAckPayload();
     
  radio.setRetries(2,15);  					//(2,15) 
  radio.setCRCLength(RF24_CRC_8);			// Use 8-bit CRC for performance
  
  radio.startListening();
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  
  
  radio.powerUp();                        //Power up the radio

//////////////
radio.writeAckPayload(1, &AckPayload, sizeof(AckPayload)); // load the payload for the FIRST time


}


	 
 
void GetData(void)
{


   	
    while (radio.available())
    {


uint8_t len = radio.getDynamicPayloadSize();
uint8_t receivedMessage[len] = {0};

uint8_t dat8[len] = {0};
uint16_t dat16[len/2] = {0};
uint32_t dat32[len/4] = {0};

radio.read(&receivedMessage, len);

for (uint8_t i = 0; i < len; i++) {dat8[i] = receivedMessage[i];}
for (uint8_t i = 0; i < len/2; i ++) {dat16[i] = (dat8[i*2] << 8) | dat8[i*2+1];}
for (uint8_t i = 0; i < len/4; i ++) {dat32[i] = dat8[i*4] | (dat8[i*4+1] << 8) | (dat8[i*4+2] << 16) | (dat8[i*4+3] << 24);}

	
 //printf(" : %u, %u, %u, %u, %u, %u, %u, %u \n\r", dat32[0], dat16[2], dat16[3], dat16[4], dat16[5], dat16[6], dat16[7], len);
 //printf(" Paylod we got: %u, %u, %u, %u, %u, %u, %u, %u \n\r", dat32[0], dat16[2], dat16[3], dat16[4], dat16[5], dat16[6], dat16[7], len);



if (dat32[0]==UINT32_MAX) { // configuration exchange with arduino
	

//printf(" Paylod we got: %u, %u, %u, %u, %u, %u, %u, %u \n\r", dat32[0], dat32[1], dat32[2], dat32[3], dat32[4], dat32[5], dat32[6], len);

AckPayload[0] = dat32[0];
AckPayload[1] = dat32[1];
AckPayload[2] = dat32[2];
time ( &rawtime ); 
AckPayload[3] = rawtime+7200; //two hours plus for actual time zone

printf("ACK payload to send next time %u, %u, %u, %u \n\r", AckPayload[0], AckPayload[1], AckPayload[2], AckPayload[3]);

radio.writeAckPayload(1, &AckPayload, sizeof(AckPayload)); // load the payload for the next time
	} //end if (dat16[0]==0)




	
if (dat32[0]!=UINT32_MAX && dat32[0] !=0) {
counter ++;

dat32[0] = ((uint32_t)dat16[0] << 16) | dat16[1];	

//printf(" Paylod we got: %u, %u, %u, %u, %u, %u, %u, %u \n\r", dat32[0], dat16[2], dat16[3], dat16[4], dat16[5], dat16[6], dat16[7], len);


if(millis() - rxTimer > 1000){ //>1000
     rxTimer = millis();
     float numBytes = counter*len;
     printf("RPI got another %u lines", counter);
     printf(" (%.2f Kbyte/s) ",numBytes/1000);
     printf(" : %u, %u, %u, %u, %u, %u, %u, %u \n\r", dat32[0], dat16[1], dat16[2], dat16[3], dat16[4], dat16[5], dat16[6], len);
     counter = 0;   
   } // enf IF
  

     
// temporaly(!) output file.
//ofstream out("output.csv"); //new file
ofstream out("output.cvs" , std::ios_base::app); //append to an existing file
out << dat32[0]<< ";" << dat16[2] << ";" <<  dat16[3] << ";" <<  dat16[4] << ";" <<  dat16[5] << ";" <<  dat16[6] << ";" <<  dat16[7] << "\n";

out.close ();

}
//delay(1); //

    } //end of "while (radio.available())"
    

    
} //end void (loop)
 
 
 
int main(int argc, char** argv) 
{

cout << "Driver initialized\n";

    setup();
    while(1) GetData();
	
  
cout << "Please check values of output file" << endl;

 
    return 0;
}
