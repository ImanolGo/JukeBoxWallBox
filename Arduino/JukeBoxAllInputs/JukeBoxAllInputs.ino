
 /*
  
  JukeBoxAllInputs
     
 Description:
 * Reading from 32 butons
 * Showing pin's Rise and Fall events

 Software:
 * RadioHead (RH_RF9x) Library  
 
 Hardware:
* Adafruit Feather M0 RFM96 LoRa Radio 433MHz
* SparkFun SX1509 IO Expansion module

 created 29th July 2019
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2019) by Imanol Gómez

 */

#include "MyBounce2.h"
#include <SparkFunSX1509.h> // Include SX1509 library
#define NUM_BUTTONS   16

 SX1509 io1;
 SX1509 io2;
 
Bounce buttons1[NUM_BUTTONS]; 
Bounce buttons2[NUM_BUTTONS];

void setupSerial()
{
    Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Starting JukeBoxAllInputs...");
}


void initializeIO()
{
    Serial.println("JukeBoxAllInputs::initializeInputs"); 
    if (!io1.begin(0x3E))
      {
        while (1) ;
      }

      Serial.println("JukeBoxAllInputs:: connected to io1");
      
       if (!io2.begin(0x3F))
      {
        while (1) ;
      }

      Serial.println("JukeBoxAllInputs:: connected to io2");     
}

void initializeButtons()
{   

    Serial.println("JukeBoxAllInputs::initializeButtons!");  
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

 void setup() 
{   
    pinMode(LED_BUILTIN, OUTPUT);
    setupSerial();
    initializeIO();
    initializeButtons();
    
    
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("JukeBoxAllInputs:: setup done");
    
}

void updatePins()
{
    for(int i = 0; i< NUM_BUTTONS; i++)
    {
        
        buttons1[i].update();
        if(buttons1[i].fell()){
          Serial.print("JukeBoxAllInputs::io1 -> FALLING EDGE = ");  
          Serial.println(i);
        }
        if(buttons1[i].rose()){
          Serial.print("JukeBoxAllInputs::io1 -> RISING EDGE = ");  
          Serial.println(i);
        }
    }

    for(int i = 0; i< NUM_BUTTONS; i++)
    {
        
        buttons2[i].update();
        if(buttons2[i].fell()){
          Serial.print("JukeBoxAllInputs::io2 -> FALLING EDGE = ");  
          Serial.println(i);
        }
        if(buttons2[i].rose()){
          Serial.print("JukeBoxAllInputs::io2 -> RISING EDGE = ");  
          Serial.println(i);
        }
    }
}

void loop() 
{
    updatePins();
}
