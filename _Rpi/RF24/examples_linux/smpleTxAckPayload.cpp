 

// SimpleTxAckPayload - the master or the transmitter

#include <cstdlib>
#include <iostream>
#include "../RF24.h"
#include <fstream> 

RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);


const uint64_t pipe = { 0xABCDABCD71LL};


//char dataToSend[10] = "Message 0";
uint32_t dataToSend;
char txNum = '0';
time_t rawtime;
int ackData[2] = {-1, -1}; // to hold the two values coming from the slave
bool newData = false;

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1; // send once per second




//=================

void showData() {
    if (newData == true) {
        printf("  Acknowledge data : %u", ackData[0]);
        printf(", %d", ackData[1]);
        printf("\n\r");
        newData = false;
    }
}

//================


//================

void send() {

    //bool rslt;
    //rslt = radio.write(&dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    printf("Data Sent ");
    time ( &rawtime ); dataToSend = rawtime;
    printf("%u ", dataToSend);  printf("\n\r");
    if (radio.write(&dataToSend, sizeof(dataToSend) )) {
        if ( radio.isAckPayloadAvailable() ) {
            radio.read(&ackData, sizeof(ackData));
            newData = true;
        }
        else {
            printf("  Acknowledge but no data "); printf("\n\r"); delay (1000);
        }

    }
    else {
        printf("  Tx failed"); printf("\n\r"); delay(1000);
    }

    prevMillis = millis();
 }



//===============

void setup () {

   
    printf("Source File /mnt/sdb1/SGT-Prog/Arduino/ForumDemos/nRF24Tutorial/SimpleTxAckPayload.ino"); printf("\n\r");
    printf("SimpleTxAckPayload Starting"); printf("\n\r");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );

    radio.enableAckPayload();

    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(pipe);
    radio.stopListening();
}

//=============

void loop() {

    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
    }
    showData();
}

//================





int main(int argc, char** argv) 
{
    setup();
    while(1)
        loop();
 
    return 0;
}

