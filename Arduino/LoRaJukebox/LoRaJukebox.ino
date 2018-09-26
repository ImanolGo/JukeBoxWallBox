
 /*
  
  LoRa JukeBox
     
 Description:
 * Reading from 26 switches and 3 registers
 * Processing switches into button events and sending them over LoRa communication
 * Processing registers into 4 different modes and  sending them over LoRa communication
 * Reading from LoRa communication to activate a relay on demand

 Software:
 * RadioHead (RH_RF9x) Library 
 * SparkFun SX1509 Library 
 
 Hardware:4 
* Adafruit Feather M0 RFM96 LoRa Radio 433MHz


 created 21th September 2018
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gómez

 */
 

#include "LoraManager.h"
#include "IOManager.h"


LoraManager loraManager;
IOManager ioManager(&loraManager);

void setupSerial()
{
    Serial.begin(115200);
    delay(1500);
    Serial.println("Starting Lora Sender!!!");
}
void setup() 
{
    setupSerial();
    ioManager.setup();
    loraManager.setup();

    Serial.println("Setup Done!!!");
}

void loop() 
{
    ioManager.update();
    loraManager.update();

    //delay(10);
    //Serial.println("Loop!!!");
 
}
