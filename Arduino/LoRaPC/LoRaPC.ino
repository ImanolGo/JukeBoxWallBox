
 /*
  
  LoRa PC
     
 Description:
 * Reading from LoRa messages and sending to the serial port 
 * The PC receives the serial commands and acts upon them
 * 
 Software:
 * RadioHead (RH_RF9x) Library 

 Hardware:4 
* Adafruit Feather M0 RFM96 LoRa Radio 433MHz


 created 21th September 2018
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gómez

 */
 

#include "LoraManager.h"
#include "SerialManager.h"


LoraManager loraManager;
SerialManager serialManager(&loraManager);


void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
    serialManager.setup();
    loraManager.setup();

    #ifdef DEBUG
      Serial.println("LoraPC::Setup Done!!!");
    #endif

    digitalWrite(LED_BUILTIN, HIGH);

    
}

void loop() 
{
    loraManager.update();
    serialManager.update();
    

    //delay(10);
    //Serial.println("Loop!!!");
 
}
