/*=============================================
   global variables
  =============================================== */
uint16_t g_clipID = 01;                                   // Clip ID
uint32_t g_maxPing = 5000;                               // Maximum time difference (µs) for successful ping
uint32_t g_maxMeasurement = 1000 * 60 * 5;               // Maximum log time is 5 Minutes
uint32_t g_timeout = 1000 * 20;
uint16_t g_logInterval = 10;                               // 15.625  milliseconds between analog entries (64Hz)
char g_FileName[15];                                      //file name to exchange
uint32_t g_RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};          // Store the last radio msg
uint16_t g_SendMsg[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//uint32_t g_SendMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t PayloadLen = 32;
uint16_t g_fileCount = 0;

uint16_t g_DataSensors[16] = {0, 0, 0, 0, 0, 0};

volatile bool IamInOven = false;
volatile bool IamAtInlet = false ;

uint16_t g_task = 0;
uint16_t task = 0;

float g_YawOffset = 0;



struct PayloadStructure {
    uint16_t Cell_ZERO;    // just take a place to cut size of Cell 0
    uint16_t Cell_0;       // g_clipID or idTask
    uint32_t Cell_1_2;     //now.unixtime() or othe long value
    uint16_t Cell_3;        //LineNumber
    uint16_t Cell_4;        // Sensor 1..15
    uint16_t Cell_5;
    uint16_t Cell_6;
    uint16_t Cell_7;
    uint16_t Cell_8;
    uint16_t Cell_9;
    uint16_t Cell_10;
    uint16_t Cell_11;
    uint16_t Cell_12;
    uint16_t Cell_13;
    uint16_t Cell_14;
    uint16_t Cell_15;

    
};

  struct PayloadStructure Payload;
/*=========================================================================
    Sensor config
    -----------------------------------------------------------------------*/
#include <SAMD_AnalogCorrection.h>


#include <ResponsiveAnalogRead.h>
ResponsiveAnalogRead analog1(A1, true);
ResponsiveAnalogRead analog2(A2, true);
ResponsiveAnalogRead analog3(A3, true);
ResponsiveAnalogRead analog4(A4, true);
ResponsiveAnalogRead analog5(A5, true);

#include <SparkFunMPU9250-DMP.h>
MPU9250_DMP imu;

#include "avdweb_AnalogReadFast.h"

bool  ANALOG_READ_FAST = 0;    // if you want try analogReadFast function - set 1 otherwise 0. Difference in function 1700 vs 3800 ms if use ResponsiveAnalogRead smoothing
bool  RESPONSIVE_ANALOG_READ = 1; // if you want try ResponsiveAnalogRead smoothing - set 1 otherwise 0. 

#define INTERRUPT_PIN_INLET  0  // 20 for version 1 Please Check!!!!
#define INTERRUPT_PIN_CLIP  1   // 21 for version 1
  
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
SdFile datfile;
#define error(s) sd.errorHalt(F(s))   // store error strings in flash to save RAM
char buf[256];               //buffer to format data - easier to echo to serial

/*=========================================================================
    Serial Monitoring
    -----------------------------------------------------------------------*/
ArduinoOutStream cout(Serial);  // Serial print stream
#define ECHO_TO_SERIAL  1   // echo data to serial port - !!! can reach stuck INTERRUPT inside StartLogging function 

/*=========================================================================
    RADIO RF24
    -----------------------------------------------------------------------*/
#include "nRF24L01.h" //libry modified from original. take care to use needed one
#include "RF24.h"
RF24 radio(6, 10); // pin 9 powered by 2V. so i deside avoid it. tested pins - 5,6,9,10 with no diffrence found
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544D52687CLL };


/*=========================================================================
                          SETUP
    -----------------------------------------------------------------------*/

void setup() {
  Serial.begin(115200);
 // while (!Serial);


  /*  =====   here is setup for AnalogRead values. set best for all four sensors. Read manual at https://github.com/dxinteractive/ResponsiveAnalogRead =====  */

  analogReadCorrection(4, 2060);//DOA 28.11.2018
  analogReadResolution(12); //12-bit resolution for analog inputs
  analogWriteResolution(10); //10-bit resolution for analog output A0
  analogReference(AR_INTERNAL2V23); //a built-in 2.23V reference


SetupSensors ();


  /*======= LED indication setup =======*/

  pinMode(8, OUTPUT); //GreenLED on board
  digitalWrite(8, LOW);
  pinMode(13, OUTPUT); //RedLED on board
  digitalWrite(13, LOW);
  pinMode(5, OUTPUT); //SLEEP pin for INA and Hall Sensors
  digitalWrite(5, HIGH);
   Serial.println ("digitalWrite(5, HIGH)");

  /*======= Radio Setup =======*/
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setChannel(111);
  radio.setPALevel (RF24_PA_HIGH);
  radio.setDataRate (RF24_1MBPS);
  radio.setAutoAck(1);                        // Ensure autoACK is enabled
  radio.enableAckPayload();                   // dual side communication for PING mode
  radio.setRetries(1, 5);                     // Optionally, increase the delay between retries in 250us & # of retries up to 15
  radio.setCRCLength(RF24_CRC_8);             // Use 8-bit CRC for performance)
  radio.openWritingPipe(pipes[0]);            // Where we send data out. Defoult Pipes[0]!!!!!

  radio.startListening();
  radio.stopListening();


  /*======= Interrupt Setup =======*/
  pinMode(INTERRUPT_PIN_INLET, INPUT_PULLUP);                   //Interrupt for sensor at TDO inlet
  pinMode(INTERRUPT_PIN_CLIP, INPUT_PULLUP);                   //Interrupt for sensor at TDO Clip (inlet and outlet)



  /* SD card SETUP is in external function SdCardErrorsCheck();.
    This function is to preven frrezing on errors.
    Use it at begin of every function thay use SD card.
    Use it inside errors-check when work with file */

  SdCardErrorsCheck(); // Setup SD card and check if card inside

  //  /*======= SD Card Setup =======*/
  //  if (!sd.begin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {    //Initialize the highest speed supported by the board
  //    sd.initErrorHalt();                               //but not over 50 MHz
  //  }

  /*======= RTC Setup =======*/
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));      //Set the time to compiled time

  //=======place to test functions ===========////

//Serial.println( "Setup done!");
//DOAtests(); // check !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

CreateFileList();

} //end SETUP()



/*=========================================================================
                          LOOP
    -----------------------------------------------------------------------*/

void loop() {

delay(1000);

  g_task = 0;
  mode_ping(task);
  task = 0;

  switch (g_task) {
    case 0:
    task=0;
      break;

    case 10:
      break;

    case 20:  //Loggin Mode
    
attachInterrupt(INTERRUPT_PIN_INLET, IRQ1, FALLING); //atach interapt to wait we are in oven...
attachInterrupt(INTERRUPT_PIN_CLIP, IRQ2, FALLING); //atach interapt to wait we are in oven...
    
      Serial.println("======= Sync Time ============");
      synctime(g_RcvMsg[2]);
      Serial.println("======= Start Measurement ============");
      StartMesurment();
      Serial.println("======= Update List ============");
      CreateFileList();
     
detachInterrupt(INTERRUPT_PIN_INLET); // Detach interrupt because end of function
detachInterrupt(INTERRUPT_PIN_CLIP); // Detach interrupt because end of function

      break;

    case 30:
      Serial.println("======= Get List============");
      CreateFileList();
      strncpy(g_FileName, "files/file.dir", 15); // use strncpy() tu put file name in *char variable 
      SendFileList(g_FileName, 1, 65535, g_task);
      task = 39; // Finished Sending 
      break;

    case 40: 
      Serial.println("======= Send file ============");
      sprintf(g_FileName, "%10lu.dat", g_RcvMsg[3]); // name file as a seconds() since 01.01.1970. // by deafault %u changed to %lu by compilation warning
      SendDatFile(g_FileName, g_RcvMsg[4], g_RcvMsg[5], g_task);
      task = 49; // Finished Sending 
      break;

    case 50: 
      Serial.println("======= Delete file ============");
      sprintf(g_FileName, "%10lu.dat", g_RcvMsg[3]); // name file as a seconds() since 01.01.1970. // by deafault %u changed to %lu by compilation warning
      DeleteFile(g_FileName);
      task = 59; // Finished deleting 
      break;

    case 60:
      Serial.println("======= Send Online ============");
      SendOnline();
      break;
      
    case 99:
      Serial.println("======= 99 ============");
      radio.flush_tx();
      radio.flush_rx();
      task = 0;
      break;
      
    default:

      break;

  }
}// end loop
