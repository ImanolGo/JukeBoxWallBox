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

    uint8_t getOutputChannel(){return  output_channel;}

    uint8_t getOutputValue(){return  output_value;}

    bool sendButtonPressed(uint8_t id, uint8_t mode);


  private:

    void initializeLora();
    void updateLora();
    bool parseMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
    bool newMessage;

    String getValue(String data, char separator, int index);

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
            if( this->isData(buf, len))
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
    String message = String((char *)_buffer);
    output_channel = (uint8_t) this->getValue(message, ',', CHANNEL_INDEX).toInt();
    output_value = (uint8_t) this->getValue(message, ',', VALUE_INDEX).toInt();
    
    
    Serial.print("LoraManager::parseMessage -> output channel = "); Serial.print(output_channel);
    Serial.print(", value = "); Serial.println(output_value);

    newMessage = true;
    return true;
}

bool LoraManager::isData(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[0] == 'd') { 
      return true;
    }

    return false;
}

 bool LoraManager::sendButtonPressed(uint8_t id, uint8_t mode)
 {
    String message = "d,";
    message+= String(id, DEC); 
    message+= ",";
    message+= String(mode, DEC); 

    Serial.print("LoraManager::sendButtonPressed -> id = "); Serial.print(id);
    Serial.print(", mode = "); Serial.println(mode);
    
    return this->sendMessage((uint8_t*)message.c_str(), message.length());
 }

bool LoraManager::sendMessage(uint8_t* _buffer, uint8_t bufferSize)
{   
     RH_RF95::printBuffer("Send: ", _buffer, bufferSize);
    return this->rf95->send(_buffer, bufferSize);    
}

String LoraManager::getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


//void LoraManager::sendButtonHold(uint8_t id, uint8_t mode)
//{       
//    return this->rf95->send(_buffer, bufferSize);    
//}
