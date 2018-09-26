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
#include <RH_RF95.h>
#include "Config.h"


class LoraManager
{
  private:
 
  RH_RF95*     rf95;        ///< the lora library
  

  public:
    
    LoraManager();
    
    void setup();
    void update();
  
    bool sendMessage(uint8_t* _buffer, uint8_t bufferSize);

    bool isNewMessage() {return  newMessage;}

    uint8_t* getBuffer(){return  received_buffer;}

    uint8_t getSize(){return  receivedLength;}


  private:

    void initializeLora();
    void updateLora();
    bool isMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool newMessage;

    uint8_t received_buffer[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t receivedLength;

};


LoraManager::LoraManager()
{
    this->rf95 = new RH_RF95(RFM95_CS, RFM95_INT); 
    newMessage= false;
    receivedLength = 0;
}

void LoraManager::setup()
{
    Serial.println("LoraManager::setup");
    initializeLora();
}
    
void LoraManager::initializeLora()
{
    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
  
    while (!this->rf95->init()) {
      Serial.println("LoraManager::LoRa radio init failed");
      while (1);
    }

    Serial.println("LoraManager::LoRa radio init OK!");

     // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
    if (!this->rf95->setFrequency(RF95_FREQ)) {
      Serial.println("LoraManager::setFrequency failed");
      while (1);
    }
    //Serial.print("LoraManager::Set Freq to: "); //Serial.println(rf69_FREQ);
    
   // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  
    this->rf95->setTxPower(23, false);
}





void LoraManager::update()
{
  newMessage = false;
  this->updateLora();
  
}

void LoraManager::updateLora()
{
      if (this->rf95->available())
      {
        // Should be a message for us now
//        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//        uint8_t len = sizeof(buf);
      
        if (this->rf95->recv(received_buffer, &receivedLength))
        {
             RH_RF95::printBuffer("Received: ", received_buffer, receivedLength);
            if(this->isMessage(received_buffer,receivedLength))
            {
                newMessage = true;
            }
        }
        else
        {
          Serial.println("LoraManager::Receive failed");
        }
      }
}



bool LoraManager::isMessage(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[0] == 0x10 && _buffer[1] == 0x41 && _buffer[2] == 0x37) 
    { 
        uint8_t data_size = _buffer[SIZE_INDEX];
        if ( (bufferSize-HEADER_SIZE) == data_size ) 
        {
          return true; 
        }
    }

    return false;
}

bool LoraManager::sendMessage(uint8_t* _buffer, uint8_t bufferSize)
{       
    return this->rf95->send(_buffer, bufferSize);    
}
