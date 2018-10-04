/*=============================================
   global variables
  =============================================== */
uint32_t g_clipID = 01;                                   // Clip ID
uint32_t g_maxPing = 5000;                               // Maximum time difference (µs) for successful ping
uint32_t g_maxMeasurement = 1000 * 60 * 90;               // Maximum log time is 5 Minutes
uint32_t g_timeout = 1000 * 20;
uint16_t g_logInterval = 10;                               // 15.625  milliseconds between analog entries (64Hz)
char g_FileName[15];                                      //file name to exchange
uint32_t g_RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};          // Store the last radio msg
uint16_t g_SendMsg[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//uint32_t g_SendMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t PayloadLen = 32;

uint16_t g_DataSensors[16] = {0, 0, 0, 0, 0, 0};

volatile bool IamInOven = false;
volatile bool IamAtInlet = false ;

uint16_t g_task = 0;
uint16_t task = 0;

float g_ARef = 3.3;
float g_AnalogToMV = g_ARef / 4096 * 1000;

/*=========================================================================
    Sensor config
    -----------------------------------------------------------------------*/
#include <SAMD_AnalogCorrection.h>


#include <ResponsiveAnalogRead.h>
ResponsiveAnalogRead analog1(A1, true);
ResponsiveAnalogRead analog2(A2, true);
ResponsiveAnalogRead analog3(A3, true);
ResponsiveAnalogRead analog4(A4, true);
ResponsiveAnalogRead analog7(A5, true);


#include <SparkFunMPU9250-DMP.h>
MPU9250_DMP imu;

#include "avdweb_AnalogReadFast.h"

bool  ANALOG_READ_FAST = 1;    // if you want try analogReadFast function - set 1 otherwise 0. Difference in function 1700 vs 3800 ms if use ResponsiveAnalogRead smoothing
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
while (!Serial);


  /*  =====   here is setup for AnalogRead values. set best for all four sensors. Read manual at https://github.com/dxinteractive/ResponsiveAnalogRead =====  */



  analogReadCorrection(26, 2062);//DOA feather m0
  //analogReadCorrection(8, 2053); //BMS feather. done  12.07.2018
  analogReadResolution(12); //12-bit resolution for analog inputs
  analogWriteResolution(10); //10-bit resolution for analog output A0
 // analogReference(AR_EXTERNAL);// external signal for analog reference
 // analogWrite(A0, 755); //2.5V for Aref input. A0 conected to Aref input. Change later if Aref connected to external reference.

SetupSensors ();

  /*======= LED indication setup =======*/

  pinMode(8, OUTPUT); //GreenLED on board
  digitalWrite(8, LOW);
  pinMode(13, OUTPUT); //RedLED on board
  digitalWrite(13, LOW);
  pinMode(5, OUTPUT); //RedLED on board
  digitalWrite(13, LOW);

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
  attachInterrupt(INTERRUPT_PIN_INLET, IRQ1, FALLING);

  pinMode(INTERRUPT_PIN_CLIP, INPUT_PULLUP);                   //Interrupt for sensor at TDO Clip (inlet and outlet)
  attachInterrupt(INTERRUPT_PIN_CLIP, IRQ2, FALLING);


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
      break;

    case 10:
      break;

    case 20:
      Serial.println("======= Logging Mode ============");
      synctime(g_RcvMsg[2]);
      StartMesurment();
      break;

    case 30:
      Serial.println("======= Get List============");
      CreateFileList();
      strncpy(g_FileName, "files/file.dir", 15); // use strncpy() tu put file name in *char variable 
      SendFile(g_FileName, 1, 65535, g_task);
      task = 39; // Finished Sending 
      break;

    case 40:
      Serial.println("======= Send file ============");
      sprintf(g_FileName, "%10lu.csv", g_RcvMsg[3]); // name file as a seconds() since 01.01.1970. // by deafault %u changed to %lu by compilation warning
      SendFile(g_FileName, g_RcvMsg[4], g_RcvMsg[5], g_task);
      task = 49; // Finished Sending 
      break;

    case 41: /// anothe file length
      Serial.println("======= Send file ============");
      sprintf(g_FileName, "%10lu.csv", g_RcvMsg[3]); // name file as a seconds() since 01.01.1970. // by deafault %u changed to %lu by compilation warning
      SendFile(g_FileName, g_RcvMsg[4], g_RcvMsg[5], g_task);
      task = 49; // Finished Sending 
      break;


    case 50:
      Serial.println("======= del file ============");
      break;

    case 60:
      Serial.println("======= Send Online ============");
      SendOnline();
      break;

    default:

      break;

  }
}
