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
  
    bool sendMessage(String message);

    bool isNewMessage() {return  newMessage;}

    uint8_t* getBuffer(){return  received_buffer;}

    uint8_t getSize(){return  receivedLength;}


  private:

    void initializeLora();
    void updateLora();
    void copyBuffer(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
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
    #ifdef DEBUG
      Serial.println("LoraManager::setup");
    #endif
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
       #ifdef DEBUG
        Serial.println("LoraManager::LoRa radio init failed");
        #endif
      while (1);
    }

    //Serial.println("LoraManager::LoRa radio init OK!");

     // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
    if (!this->rf95->setFrequency(RF95_FREQ)) {
      #ifdef DEBUG
        Serial.println("LoraManager::setFrequency failed");
       #endif
     
      while (1);
    }

     #ifdef DEBUG
        Serial.print("LoraManager::Set Freq to: "); Serial.print(RF95_FREQ); Serial.println("MHz");
      #endif

    
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
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        
      
        if (this->rf95->recv(buf, &len))
        {
             digitalWrite(LED_BUILTIN, HIGH);
             #ifdef DEBUG
                Serial.print("LoraManager::Received Length: ");  Serial.println(len);
              #endif
              
             #ifdef DEBUG
              RH_RF95::printBuffer("Received: ", buf, len);
            #endif

            
   
             //newMessage = true;
            if(this->isData(buf,len))
            { 
                this->copyBuffer(buf, len);
                newMessage = true;
            }
        }
        else
        {
             #ifdef DEBUG
                Serial.println("LoraManager::Receive failed");
              #endif
        }
      }
}


void LoraManager::copyBuffer(uint8_t* _buffer, uint8_t bufferSize)
{     
      if(RH_RF95_MAX_MESSAGE_LEN<bufferSize){
        return;
      }

      receivedLength = bufferSize;
       
      for(int i=0; i<receivedLength; i++){
        received_buffer[i] = _buffer[i];
      }
    
}

bool LoraManager::isData(uint8_t* _buffer, uint8_t bufferSize)
{

//    #ifdef DEBUG
//          Serial.print("LoraManager::_buffer[0]-> "); Serial.println(_buffer[0]);
//          Serial.print("LoraManager::_buffer[1]-> "); Serial.println(_buffer[1]);
//          Serial.print("LoraManager::_buffer[2]-> "); Serial.println(_buffer[2]);
//     #endif

     ;
        
    if ( _buffer[0] == 'd') 
    { 

       #ifdef DEBUG
                Serial.println("LoraManager::isMessage -> true");
        #endif
            
                return true; 
     }
     

     #ifdef DEBUG
          Serial.println("LoraManager::isMessage -> false");
     #endif
        
    return false;
}

bool LoraManager::sendMessage(String message)
{       
    return this->rf95->send((uint8_t*)message.c_str(), message.length());    
}
