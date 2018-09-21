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
#include "MyButtonEvents.h"

class InputManager
{
  private:
 
  LoraManager* loraManager;  ///< it sends the data to the lora manager
  

  public:
    
    InputManager(LoraManager* loraManager);
    
    void setup();
    void update();

  private:

    void initializeInputs();
    void updateSerial();
    void parseMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
    bool isConnection(uint8_t* _buffer, uint8_t bufferSize);
    void sendConnected();


  private:
 
    SX1509* io1;
    SX1509* io2;

    MyButtonEvents buttons[NUM_BUTTONS]; 
  
};


InputManager::InputManager(LoraManager* loraManager)
{
    this->loraManager=loraManager;
}

void InputManager::setup()
{
    initializeInputs();
    initializeButtons();
    Serial.println("InputManager::setup");
}

void InputManager::initializeInputs()
{
     io1 = new SX1509();
     io2 = new SX1509();

      if (!io1->begin(0x3E))
      {
        while (1) ;
      }

      Serial.println("InputManager::initializeInputs -> connected to io1");
      
       if (!io2->begin(0x3F))
      {
        while (1) ;
      }

      Serial.println("InputManager::initializeInputs -> connected to io2");     
}

void InputManager::initializeButtons()
{   
    int id = 0;
    for(int i = )
}

void InputManager::update()
{
    updateSerial();
}

void InputManager::updateSerial()
{     

    uint8_t numBytes = Serial.available();
    
    if (numBytes > 0) 
    {
         //Serial.print("SerialManager::received -> ");
         //Serial.print(numBytes);
         //Serial.println(numBytes);
         //Serial.print("OK"); 
         uint8_t buf[numBytes];
         Serial.readBytes((char *) buf,numBytes);
         //Serial.print("OK");
         this->parseMessage(buf, numBytes);
    }
    
}


void InputManager::parseMessage(uint8_t* buf, uint8_t len)
{
      if(this->isMessage(buf,len))
      {
          if(this->isConnection(buf, len)){
            this->sendConnected();
            
          }
          else if(this->isData(buf, len)){
              this->loraManager->sendMessage(buf, len);
          } 
         
      }
}


void InputManager::sendConnected()
{
      int len = HEADER_SIZE+1;
      byte buf[len];
      buf[0] = 0x10;
      buf[1] = 0x41;
      buf[2] = 0x37;
      buf[3] = 1;
      buf[4] = 'c';
      buf[5] = 0;
      buf[6] = 'c';
      
      Serial.write(buf, len);
      this->loraManager->sendMessage(buf, len);
      _connected = true;
}


bool InputManager::isMessage(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[0] == 0x10 && _buffer[1] == 0x41 && _buffer[2] == 0x37) 
    { 
        uint8_t data_size = _buffer[SIZE_INDEX];
        if ( (bufferSize-HEADER_SIZE) == data_size ) 
        {
           //Serial.println("SerialManager::isMessage -> true");
          return true; 
        }
    }

    //Serial.println("SerialManager::isMessage -> false");
    return false;
}


bool InputManager::isData(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[COMMAND_INDEX] == 'd') { 
      return true;
    }

    return false;
}


bool InputManager::isConnection(uint8_t* _buffer, uint8_t bufferSize)
{
    if ( _buffer[COMMAND_INDEX] == 'c') { 
       //Serial.println("SerialManager::isConnection -> true");
      return true;
    }

    //Serial.println("SerialManager::isConnection -> false");
    return false;
}



