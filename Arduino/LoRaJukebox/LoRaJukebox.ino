
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
 
 Hardware:
* Adafruit Feather M0 RFM96 LoRa Radio 433MHz
* SparkFun SX1509 IO Expansion module


 created 21th September 2018
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gómez

 */
 

#include "LoraManager.h"
#include "IOManager.h"


LoraManager loraManager;
IOManager ioManager(&loraManager);

unsigned long StartTime = millis();


void setupSerial()
{
    randomSeed(analogRead(0));
    Serial.begin(115200);
    delay(1500);
    Serial.println("Starting Lora Sender!!!");
}
void setup() 
{   
    pinMode(LED_BUILTIN, OUTPUT);
    setupSerial();
    ioManager.setup();
    loraManager.setup();
    
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Setup Done!!!");
}

void loop() 
{   
    //Serial.println(millis());
    ioManager.update();
    loraManager.update();

    #ifdef DEBUG
    //Serial.println(millis());
    if(millis() - StartTime > 20000){
        StartTime = millis();
        Serial.print("Time -> ");
        Serial.println(StartTime/1000);
        Serial.println(loraManager.sendButtonPressed( random(26), random(4)));
    }

    #endif
 
}
