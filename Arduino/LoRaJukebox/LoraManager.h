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

    bool getOutputChannel(){return  output_channel;}

    bool getOutputValue(){return  output_value;}

    bool sendButtonPressed(uint8_t id, uint8_t mode);


  private:

    void initializeLora();
    void updateLora();
    bool parseMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
    bool newMessage;

    uint8_t output_channel;
    uint8_t output_value;

};


LoraManager::LoraManager()
{
    this->rf95 = new RH_RF95(RFM95_CS, RFM95_INT); 
    newMessage= false;
    output_channel = 0;
    output_value = 0;
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
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
      
        if (this->rf95->recv(buf, &len))
        {
             RH_RF95::printBuffer("Received: ", buf, len);
            if(this->isMessage(buf,len) && this->isData(buf, len))
            {
                this->parseMessage(buf,len);
                
            }
        }
        else
        {
          Serial.println("LoraManager::Receive failed");
        }
      }
}

bool LoraManager::parseMessage(uint8_t* _buffer, uint8_t bufferSize)
{
    if(bufferSize < HEADER_SIZE + DATA_BYTES ){
      return false;
    }

    output_channel = _buffer[HEADER_SIZE];
    output_value = _buffer[HEADER_SIZE+1];

    Serial.print("LoraManager::parseMessage -> output channel = "); Serial.print(output_channel);
    Serial.print(", value = "); Serial.println(output_value);

    newMessage = true;
    return true;
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

bool LoraManager::isData(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[COMMAND_INDEX] == 'd') { 
      return true;
    }

    return false;
}

 bool LoraManager::sendButtonPressed(uint8_t id, uint8_t mode)
 {
    uint8_t buffSize = HEADER_SIZE + DATA_BYTES;
    uint8_t buff[buffSize];

    buff[0] = 0x10; 
    buff[1] = 0x41; 
    buff[2] = 0x37; 
    buff[3] = DATA_BYTES; 
    buff[4] = 'd'; 
    buff[5] = id; 
    buff[6] = mode; 
    
    return this->sendMessage(buff, buffSize);
 }

bool LoraManager::sendMessage(uint8_t* _buffer, uint8_t bufferSize)
{       
    return this->rf95->send(_buffer, bufferSize);    
}


//void LoraManager::sendButtonHold(uint8_t id, uint8_t mode)
//{       
//    return this->rf95->send(_buffer, bufferSize);    
//}
