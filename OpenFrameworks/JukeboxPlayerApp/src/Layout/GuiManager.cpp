/*
 *  GuiManager.cpp
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */


#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
const float GuiManager::GUI_WIDTH = 200;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
    //Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    
    this->setupGuiParameters();
	// this->setupModesGui();
 //this->setupAudioGui();
    this->loadGuiValues();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
	auto serialManager = &AppManager::getInstance().getSerialManager();

   // m_gui.setPosition(MARGIN, MARGIN);
    //m_gui.setPosition(20, 20);
	m_gui.setup("GUI"); 
    m_gui.add(m_guiFPS.set("FPS", 0, 0, 60));
    
    m_isSerial.set("Serial", serialManager->getConnected());

	m_relay.set("Relay",false);
	m_relay.addListener(serialManager, &SerialManager::sendRelayToggle);

	m_light.set("Light", false);
	m_light.addListener(serialManager, &SerialManager::sendLightToggle);
    
    m_gui.add(m_isSerial);
	m_gui.add(m_relay);
	m_gui.add(m_light);
    
}


void GuiManager::update()
{
   // m_gui.update();
    
    m_guiFPS = ofGetFrameRate();
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    //this->drawRectangle();
    this->drawGui();
}


void GuiManager::drawGui()
{
    ofEnableAlphaBlending();
    m_gui.draw();
    ofDisableAlphaBlending();
}

void GuiManager::saveGuiValues(string path)
{
    ofXml xml;
   // ofSerialize(xml, m_parameters);
    //xml.serialize(m_parameters);
    
    if(path.empty()){
        xml.save(GUI_SETTINGS_FILE_NAME);
    }
    else{
        xml.save(path);
    }
    
    
}

void GuiManager::loadGuiValues(string path)
{
    ofXml xml;
    if(path.empty()){
         xml.load(GUI_SETTINGS_FILE_NAME);
    }
    else{
         xml.load(path);
    }
    
    //xml.deserialize(m_parameters);
    //ofDeserialize(xml, m_parameters);
}

void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::setAudioMode(int value)
{
	if (value < 0 || value >= AudioManager::NUM_MODES) {
		return;
	}

	string dropBoxName = "MODES";
	AppManager::getInstance().getAudioManager().changeMode(value);
}

void GuiManager::setAudioIndex(int value)
{
	if (value < 0 || value >= AudioManager::NUM_SAMPLES) {
		return;
	}

	string dropBoxName = "SAMPLES";
	AppManager::getInstance().getAudioManager().changeSample(value);
}

void GuiManager::setSerialConnected(bool value)
{
    m_isSerial = value;
}

void GuiManager::setRelayValue(bool value)
{
	m_relay = value;
}

void GuiManager::setLightValue(bool value)
{
	m_light = value;
}

