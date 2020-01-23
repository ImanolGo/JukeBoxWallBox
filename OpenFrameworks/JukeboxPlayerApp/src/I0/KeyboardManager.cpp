/*
 *  KeyboardManager.cpp
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */


#include "KeyboardManager.h"
#include "AppManager.h"


KeyboardManager::KeyboardManager(): Manager()
{
    //Intentionally left empty
}

KeyboardManager::~KeyboardManager()
{
   ofLogNotice() << "KeyboardManager::destructor";
}


//--------------------------------------------------------------

void KeyboardManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"KeyboardManager::initialized" ;
    this->addListeners();
}

void KeyboardManager::addListeners()
{
    ofRegisterKeyEvents(this); // this will enable the InteractiveVisual class to listen to the key events.
}

void KeyboardManager::keyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    
    //ofLogVerbose() <<"KeyboardManager::keyPressed-> " + ofToString(key);
    
    
}

void KeyboardManager::keyReleased(ofKeyEventArgs &e)
{
    int key = e.key;
    
    //ofLogVerbose() <<"KeyboardManager::keyReleased-> " + ofToString(key);
    
}










