///////////////////////////////////////////////////////////////////
// Class controlling the Serial communication
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#pragma once
#include "Arduino.h"
#include "LoraManager.h"
#include "Config.h"

class SerialManager
{
  private:

  LoraManager* loraManager;  ///< it sends/reads the commands to/from the lora manager
 
  public:
    
    SerialManager(LoraManager* loraManager);
    
    void setup();
    void update();

    void write(uint8_t* _buffer, uint8_t bufferSize);

  private:

    void initializeSerial();
    void updateSerial();
    void updateLora();
    void sendConnected();
    void parseMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
    bool isConnection(uint8_t* _buffer, uint8_t bufferSize);
  
    bool m_connected;

    uint8_t* m_message;
    uint8_t* m_messageBuffer;
    int  m_numBufferedChars;
    int  m_numMessageChars;
  
};


SerialManager::SerialManager(LoraManager* loraManager)
{
    this->loraManager=loraManager;
    m_connected = false;

    
    m_numBufferedChars = 0;
    m_numMessageChars = 0;
}

void SerialManager::setup()
{
    initializeSerial();

    #ifdef DEBUG
          Serial.println("SerialManager::setup");
    #endif
             
}

void SerialManager::initializeSerial()
{
    Serial.begin(115200);
    Serial.flush();
    delay(1500);
}


void SerialManager::update()
{
    updateSerial();
    updateLora();
}

void SerialManager::updateSerial()
{     
      if(Serial.available())
      {
        String message = Serial.readStringUntil('|');
        this->parseMessage((uint8_t*)message.c_str(), message.length());
        
//        while(Serial.available() > 0)
//        {
//          char readChar = Serial.read();
//          if(readChar == '|')
//          {
//            Serial.println("SerialManager::isMessage -> readChar == | ");
//            for(int i = 0;i<m_numBufferedChars;i++)
//            {
//              m_message[i] = m_messageBuffer[i];
//              m_messageBuffer[i] = 0;
//            }
//             m_numMessageChars = m_numBufferedChars;
//             m_numBufferedChars = 0;
//             digitalWrite(LED_BUILTIN, HIGH);
//             this->parseMessage(m_message, m_numMessageChars);
//             digitalWrite(LED_BUILTIN, LOW);
//          }
//          else
//          {
//            m_messageBuffer[m_numBufferedChars] = readChar;
//            m_numBufferedChars++;
//          }
//        }
      }
}


void SerialManager::updateLora()
{ 
    if(this->loraManager->isNewMessage()){
         #ifdef DEBUG
          Serial.println("SerialManager::isMessage -> true");
        #endif
        
        this->write(this->loraManager->getBuffer(), this->loraManager->getSize());
    }
}

void SerialManager::parseMessage(uint8_t* buf, uint8_t len)
{
     #ifdef DEBUG
          Serial.println("SerialManager::parseMessage");
      #endif
        
     if(this->isConnection(buf, len)){
        this->sendConnected();
        
      }
      else if(this->isData(buf, len)){
          this->loraManager->sendMessage(buf, len);
      } 
}


void SerialManager::sendConnected()
{
      Serial.print("c");
      Serial.print(",");
      Serial.print("1");
      Serial.println("");
      
      m_connected = true;
}


void SerialManager::write(uint8_t* _buffer, uint8_t bufferSize)
{
    #ifdef DEBUG
          Serial.print("SerialManager::write -> num bytes: "); Serial.println(bufferSize);
    #endif
        
    Serial.write(_buffer, bufferSize);
}

bool SerialManager::isData(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[COMMAND_INDEX] == 'd') { 
      return true;
    }

    return false;
}


bool SerialManager::isConnection(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[0] == 'c') { 
      #ifdef DEBUG
          Serial.println("SerialManager::isConnection -> true");
      #endif
      return true;
    }

    #ifdef DEBUG
          Serial.println("SerialManager::isConnection -> false");
      #endif
    return false;
}
