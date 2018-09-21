
 /*
  
  LoRa Input Sender 
     
 Description:
 * Firmware sending input changes usin the LoRa protocol

 Software:
 * FastLED Library 
 * RadioHead (RH_RF9x) Library 
 
 Hardware:
* Adafruit Feather M0 RFM96 LoRa Radio 433MHz


 created 21th September 2018
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gómez

 */
 

#include "LoraManager.h"
#include "InputManager.h"


LoraManager loraManager;
InputManager inputManager(&loraManager);

void setupSerial()
{
    Serial.begin(115200);
    delay(300);
    Serial.println("Starting Lora Sender!!!");
}
void setup() 
{
    setupSerial();
    inputManager.setup();
    loraManager.setup();
}

void loop() 
{
    inputManager.update();
    loraManager.update();
 
}

