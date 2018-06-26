/*=============================================
 * global variables
=============================================== */
int32_t g_clipID = 01;                                   // Clip ID
int32_t g_maxPing = 1000000;                               // Maximum time difference (µs) for successful ping
int32_t g_maxMeasurement = 300000;                        // Maximum log time is 5 Minutes
uint8_t g_logInterval = 15;                               // 15.625  milliseconds between analog entries (64Hz)
char g_FileName[13];                                      //file name to exchange 
uint32_t g_RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};          // Store the last radio msg
uint32_t g_SendMsg[8]= {0, 0, 0, 0, 0, 0, 0, 0};

volatile bool IamInOven=false;
volatile bool IamAtInlet= false ;

/*=========================================================================
    Sensor config
    -----------------------------------------------------------------------*/
#include <SAMD_AnalogCorrection.h>


/*=========================================================================
    real Time Config
    -----------------------------------------------------------------------*/

#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"        //libry modified from original. take care to use needed one

RTC_Millis rtc;
/*=========================================================================
    SD CARD include
    -----------------------------------------------------------------------*/
#include <SPI.h>
#include "SdFat.h"
#define SD_CHIP_SELECT  4       //hardwritten pinout on feather adalogger board
SdFat sd;                       //file system object
ofstream logfile;               // text file for logging
#define error(s) sd.errorHalt(F(s))   // store error strings in flash to save RAM
char buf[80];               //buffer to format data - easier to echo to serial

/*=========================================================================
    Serial Monitoring
    -----------------------------------------------------------------------*/
ArduinoOutStream cout(Serial);  // Serial print stream
#define ECHO_TO_SERIAL  1   // echo data to serial port - !!! can reach stuck INTERRUPT inside StartLogging function 

/*=========================================================================
    RADIO RF24
    -----------------------------------------------------------------------*/
#include "nRF24L01.h" //libry modified from original. tale care to use needed one
#include "RF24.h"
RF24 radio(6, 10); // pin 9 powered by 2V. so i deside avoid it. tested pins - 5,6,9,10 with no diffrence found
//#include<SPI.h>
//#include<RF24.h>
//const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544D52687CLL };


/*=========================================================================
                          SETUP
    -----------------------------------------------------------------------*/

void setup() {
  Serial.begin(115200);
 // while (!Serial);
  analogReadCorrection(8, 2053); //done for CURRENT m0 board
  pinMode(8, OUTPUT); //GreenLED on board
  digitalWrite(8, LOW);
  pinMode(13, OUTPUT); //RedLED on board
  digitalWrite(13, LOW);

  /*======= Radio Setup =======*/   
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setChannel(77); //125 in the past
  radio.setPALevel (RF24_PA_LOW); 
  radio.setDataRate (RF24_1MBPS);
  radio.setAutoAck(1);                        // Ensure autoACK is enabled
  radio.enableAckPayload();                   //dual side communication for PING mode
  radio.setRetries(2, 15);                    // Optionally, increase the delay between retries in 250us & # of retries up to 15
  radio.setCRCLength(RF24_CRC_8);             // Use 8-bit CRC for performance)
  radio.openWritingPipe(0xABCDABCD71LL);            // Where we send data out
  radio.startListening();
  radio.stopListening();

  /*======= Interrupt Setup =======*/   
  pinMode(20, INPUT_PULLUP);                   //Interrupt for sensor at TDO inlet
  attachInterrupt(20, IRQ1, FALLING);

  pinMode(21, INPUT_PULLUP);                   //Interrupt for sensor at TDO Clip (inlet and outlet)
  attachInterrupt(21, IRQ2, FALLING);

 /* Interrupt pins SETUP  DOA*/
//
//  IamAtInlet = false;
//  pinMode(9, INPUT_PULLUP); //Interrupt for sensor at TDO inlet
//  attachInterrupt(9, IRQ1, FALLING);
//
//  IamInOven = false;
//  pinMode(5, INPUT_PULLUP); //Interrupt for sensor at TDO Clip (inlet and outlet)
//  attachInterrupt(5, IRQ2, FALLING);


 /* SD card SETUP is in external function SdCardErrorsCheck();. 
  This function is to preven frrezing on errors. 
  Use it at begin of every function thay use SD card. 
  Use it inside errors-check when work with file */

SdCardErrorsCheck(); // Setup SD card an check if is it in

//  /*======= SD Card Setup =======*/
//  if (!sd.begin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {    //Initialize the highest speed supported by the board
//    sd.initErrorHalt();                               //but not over 50 MHz
//  }

  /*======= RTC Setup =======*/
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));      //Set the time to compiled time

//=======place to test functions ===========////

//while(!Serial);
//CreateFileList();

} //end SETUP()



/*=========================================================================
                          LOOP
    -----------------------------------------------------------------------*/

void loop(){
  if (ping()>0) {
      switch (g_RcvMsg[2]) {
          case 0:
              break;
          
          case 10:
              break;
              
          /*======= Logging Mode ============*/    
          case 20:
              synctime(g_RcvMsg[3]);
              StartMesurmentCSV();
              g_RcvMsg[2] = 0;
              break;
              
          case 30:
              break;
              
           default:
           
              break;
        
      }
    
  }
}
