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
#include "Config.h"

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
    void initializeButtons();
    void updateButtons();
    void parseMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isMessage(uint8_t* _buffer, uint8_t bufferSize);
    bool isData(uint8_t* _buffer, uint8_t bufferSize);
    bool isConnection(uint8_t* _buffer, uint8_t bufferSize);
    void sendConnected();


  private:
 
    SX1509* io1;
    SX1509* io2;

    ButtonEvents buttons[NUM_BUTTONS]; 
  
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
    for(int i = 0; i< NUM_BUTTONS/2; i++){
      
      buttons[id] = ButtonEvents(id);
      buttons[id].attach(io1, i, INPUT_PULLUP); 
      id++;
    }

    for(int i = 0; i< NUM_BUTTONS/2; i++){
      
      buttons[id] = ButtonEvents(id);
      buttons[id].attach(io2, i, INPUT_PULLUP); 
      id++;
    }
}

void InputManager::update()
{
    updateButtons();
}

void InputManager::updateButtons()
{     

   for(int i = 0; i< NUM_BUTTONS; i++){
      buttons[i].update();
      // things to do if the button was tapped (single tap)
      if (buttons[i].tapped() == true) {
        Serial.println("TAP event detected");          
      }
    
      // things to do if the button was double-tapped
      if (buttons[i].doubleTapped() == true) {
        Serial.println("DOUBLE-TAP event detected");
      }
      
      // things to do if the button was held
      if (buttons[i].held() == true) {
            Serial.println("HOLD event detected");
            //this->loraManager->sendHold(buf, len);
      }  

    }
}



