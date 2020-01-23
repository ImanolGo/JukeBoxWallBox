///////////////////////////////////////////////////////////////////
// Class controlling the Input/Output communication
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#pragma once
#include "Arduino.h"
#include "LoraManager.h"
#include "MyBounce2.h"
#include "Config.h"
#include <SparkFunSX1509.h> // Include SX1509 library


class Output
{
    public:
    
    Output();
    Output(SX1509* io, uint8_t pin); // default constructort
    void attach(SX1509* io, uint8_t pin);
    void write(byte highLow);//highLow: should be Arduino's defined HIGH or LOW constants.

    private:

    SX1509* io;
    uint8_t pin;
};


Output::Output()
{
    this->io = NULL;
    this->pin = 0;
}


Output::Output(SX1509* io, uint8_t pin)
{
    this->io = NULL;
    this->attach(io, pin);
}

void Output::attach(SX1509* io, uint8_t pin)
{
    this->io = io;
    this->pin = pin;
    this->io->pinMode(pin, OUTPUT);
}

 void Output::write(byte highLow)
 {
      if(this->io==NULL){
        return;
      }

      this->io->digitalWrite(this->pin, highLow);
 }



class IOManager
{
  private:
 
  LoraManager* loraManager;  ///< it sends the data to the lora manager
  

  public:
    
    IOManager(LoraManager* loraManager);
    
    void setup();
    void update();

    void setOutput(uint8_t id, bool value);

  private:

    void initializeIO();
    void initializeButtons();
    void initializeRegisters();
    void initializeOutputs();
    void updateButtons();
    void updateRegisters();
    void updateOutputs();


  private:
 
    SX1509 io1;
    SX1509 io2;

    Bounce buttons1[NUM_BUTTONS]; 
    Bounce buttons2[NUM_BUTTONS]; 
    Bounce registers[NUM_REGISTERS]; 
    Output       outputs[NUM_OUTPUTS]; 

    uint8_t _mode;
    uint8_t prev_mode;
  
};


IOManager::IOManager(LoraManager* loraManager)
{
    this->loraManager=loraManager;
    _mode = 0;
    prev_mode = 255;
}

void IOManager::setup()
{
    Serial.println("IOManager::setup");
    initializeIO();
    initializeButtons();
    //initializeRegisters();
    //initializeOutputs();
   
}

void IOManager::initializeIO()
{
      Serial.println("IOManager::initializeInputs");  
      
      if (!io1.begin(0x3E))
      {
        while (1) ;
      }

      Serial.println("IOManager::initializeInputs -> connected to io1");
      
       if (!io2.begin(0x3F))
      {
        while (1) ;
      }

      Serial.println("IOManager::initializeInputs -> connected to io2");     
}

void IOManager::initializeButtons()
{   

     Serial.println("IOManager::initializeButtons!");  
    for(int i = 0; i< NUM_BUTTONS; i++){
      
      buttons1[i] = Bounce();
      buttons1[i].attach(&io1, i, INPUT_PULLUP); 
      Serial.print("JukeBoxAllInputs::added button ");  
      Serial.print(i);  
      Serial.println(" to io1");  
    }

    for(int i = 0; i< NUM_BUTTONS; i++){
      
      buttons2[i] = Bounce();
      buttons2[i].attach(&io2, i, INPUT_PULLUP); 
      Serial.print("JukeBoxAllInputs::added button ");  
      Serial.print(i);  
      Serial.println(" to io2");  
    }
}


void IOManager::initializeRegisters()
{   

    Serial.println("IOManager::initializeRegisters!");  
    int id = 0;
    int offset = 16 - NUM_REGISTERS;
    for(int i = 0; i<  NUM_REGISTERS; i++){
 
      registers[id] = Bounce();
      registers[id].attach(&io2, offset+i, INPUT_PULLUP); 
      Serial.print("IOManager::added register ");  
      Serial.print(id);  
      Serial.print(", input: ");  
      Serial.print(offset+i); 
      Serial.println(" to io2");  
      id++;
    }
}

void IOManager::initializeOutputs()
{
    Serial.println("IOManager::initializeOutputs!"); 
    int offset = 15 - NUM_OUTPUTS;
    for(int i =0 ; i< NUM_OUTPUTS; i++){
      outputs[i] = Output(&io1, offset+i); 
      Serial.print("IOManager::added output ");  
      Serial.print(i);  
      Serial.print(", output: ");  
      Serial.print(offset+i); 
      Serial.println(" to io1"); 
    }
}


void IOManager::setOutput(uint8_t id, bool value)
{
    if(id>=NUM_OUTPUTS){
      return;
    }

    outputs[id].write(value);
    Serial.print("IOManager::setOutput -> id:  "); Serial.print(id);  
    Serial.print(", value: "); Serial.println(value);  
}


void IOManager::update()
{   
    updateButtons();
    //updateRegisters();
    //updateOutputs();   
}


void IOManager::updateOutputs()
{ 
    if(this->loraManager->isNewMessage()){
         this->setOutput(this->loraManager->getOutputChannel(), this->loraManager->getOutputValue());
    }
}
void IOManager::updateButtons()
{     
    int id = 0;
    for(int i = 0; i< NUM_BUTTONS; i++)
    {
        
        buttons1[i].update();
        if(buttons1[i].fell()){
          Serial.print("JukeBoxAllInputs::io1 -> FALLING EDGE = ");  
          Serial.println(i);

          this->loraManager->sendButtonPressed(id, _mode);
          Serial.print("IOManager:: -> send button pressed = ");  
          Serial.println(id);
        }
        if(buttons1[i].rose()){
          Serial.print("JukeBoxAllInputs::io1 -> RISING EDGE = ");  
          Serial.println(i);
        }

        id++;
    }

    for(int i = 0; i< NUM_BUTTONS; i++)
    {
        
        buttons2[i].update();
        if(buttons2[i].fell()){
          Serial.print("JukeBoxAllInputs::io2 -> FALLING EDGE = ");  
          Serial.println(i);

          this->loraManager->sendButtonPressed(id, _mode);
          Serial.print("IOManager:: -> send button pressed = ");  
          Serial.println(id);
        }
        if(buttons2[i].rose()){
          Serial.print("JukeBoxAllInputs::io2 -> RISING EDGE = ");  
          Serial.println(i);
        }

        id++;
    }

}


void IOManager::updateRegisters()
{     

    for(int i = 0; i< NUM_REGISTERS; i++)
    {
        registers[i].update();  
    }

    //mode = registers[0].read() + 2*registers[1].read() + 4*registers[2].read();

    if(registers[2].read() == 1){
      _mode = 3;
    }

    else if(registers[1].read() == 1)
    {
      _mode = 2;
    }

     else if(registers[0].read() == 1)
    {
      _mode = 1;
    }

    else
    {
      _mode = 0;
    }

    if(prev_mode!=_mode){
      prev_mode = _mode;
      Serial.print("IOManager::Mode: "); Serial.println(_mode);
    }

  
}
