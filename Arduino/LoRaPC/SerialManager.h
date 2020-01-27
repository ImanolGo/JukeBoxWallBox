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

    void write(String & message);

  private:

    void initializeSerial();
    void updateSerial();
    void updateLora();
    void sendConnected();
    void parseMessage(String& message);
    bool isData(String& message);
    bool isConnection(String& message);

    String getValue(String data, char separator, int index);
  
    bool m_connected;
  
};


SerialManager::SerialManager(LoraManager* loraManager)
{
    this->loraManager=loraManager;
    m_connected = false;

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
        this->parseMessage(message);

      }
}


void SerialManager::updateLora()
{ 
    if(this->loraManager->isNewMessage()){
         #ifdef DEBUG
          Serial.println("SerialManager::isMessage -> true");
        #endif

        String message = String((char*)this->loraManager->getBuffer());
        Serial.println(message);
    
        //Serial.write(this->loraManager->getBuffer(), this->loraManager->getSize());

    }
}

void SerialManager::parseMessage(String& message)
{
     #ifdef DEBUG
          Serial.println("SerialManager::parseMessage");
      #endif
        
     if(this->isConnection(message)){
        this->sendConnected();
        
      }
      else if(this->isData(message)){
//          String m = "SerialManager::parseMessage: Output -> channel = ";
//          m+= this->getValue(message,',', 1);
//          m+= ", value = ";
//          m+= this->getValue(message,',',2);
//          Serial.println(m);
          this->loraManager->sendMessage(message);
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


void SerialManager::write(String& message)
{
    #ifdef DEBUG
          Serial.print("SerialManager::write -> num bytes: "); Serial.println(message.length());
    #endif
        
    Serial.write(message.c_str(), message.length());
}

bool SerialManager::isData(String& message)
{
    if (message.charAt(0) == 'd') { 
      return true;
    }

    return false;
}


bool SerialManager::isConnection(String& message)
{
    if (message.charAt(0) == 'c') { 
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


String SerialManager::getValue(String data, char separator, int index)
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
