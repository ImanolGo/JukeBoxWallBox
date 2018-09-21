///////////////////////////////////////////////////////////////////
// Class controlling the LoRa Radio communication
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#pragma once
#include "Arduino.h"
#include <SPI.h>
#include <RH_RF69.h>


//for feather m0  
#define RFM69_CS 8
#define RFM69_RST 4
#define RFM69_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 434.0


class LoraManager
{
  private:
 
  RH_RF69*     rf69;        ///< the lora library
  

  public:
    
    LoraManager();
    
    void setup();
    void update();

    bool sendMessage(uint8_t* _buffer, uint8_t bufferSize);

  private:

    void initializeLora();
 
};


LoraManager::LoraManager()
{
    this->rf69 = new RH_RF69(RFM69_CS, RFM69_INT); 
}

void LoraManager::setup()
{
    //Serial.println("LoraManager::setup");
    initializeLora();
}

void LoraManager::initializeLora()
{
    // manual reset
    digitalWrite(RFM69_RST, LOW);
    delay(10);
    digitalWrite(RFM69_RST, HIGH);
    delay(10);
  
    while (!this->rf69->init()) {
      //Serial.println("LoraManager::LoRa radio init failed");
      while (1);
    }

    //Serial.println("LoraManager::LoRa radio init OK!");

     // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
    if (!this->rf69->setFrequency(RF69_FREQ)) {
      //Serial.println("LoraManager::setFrequency failed");
      while (1);
    }
    //Serial.print("LoraManager::Set Freq to: "); //Serial.println(rf69_FREQ);
    
    // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
    this->rf69->setTxPower(20, true);
}





void LoraManager::update()
{
}

bool LoraManager::sendMessage(uint8_t* _buffer, uint8_t bufferSize)
{       
    return this->rf69->send(_buffer, bufferSize);    
}

