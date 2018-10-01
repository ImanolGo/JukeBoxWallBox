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
#include "GuiTheme.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
//const int GuiManager::GUI_WIDTH = 350;


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
    this->setupModesGui();
    this->setupAudioGui();

    this->setupGuiEvents();
    this->loadGuiValues();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    
    ofxDatGuiLog::quiet();
    
    //m_gui.setPosition(ofxDatGuiAnchor::TOP_LEFT);
    m_gui.setPosition(0,0);
    //m_gui.setAssetPath(ofToDataPath("fonts/"));
    //m_gui.setAssetPath(ofToDataPath("fonts/"));
    //m_gui.setAssetPath("../Resources/data/fonts/");
    m_gui.setTheme(new GuiTheme());
    
    
    int margin =  LayoutManager::MARGIN;
    m_gui.setAutoDraw(false);
    auto pos = m_gui.getPosition();
    m_gui.setPosition(pos.x + margin, pos.y + margin);
    m_gui.addHeader(GUI_SETTINGS_NAME, false);
    
    m_gui.addFRM(0.1);
    
    m_gui.addToggle("Serial", AppManager::getInstance().getSerialManager().getConnected());
    
    m_gui.addBreak();
}

void GuiManager::setupModesGui()
{
    vector<string> modes;
    for(int i = 0; i <  AudioManager::NUM_MODES; i++)
    {
        string modeName = "MODE: " + ofToString(i);
        modes.push_back(modeName);
    }
    
    m_gui.addDropdown("MODES", modes);
    m_gui.addBreak();
}


void GuiManager::setupAudioGui()
{
    vector<string> samples;
    for(int i = 0; i <  AudioManager::NUM_SAMPLES; i++)
    {
        string sampleName = "SAMPLE: " + ofToString(i);
        samples.push_back(sampleName);
    }
    
    m_gui.addDropdown("SAMPLES", samples);
    m_gui.addBreak();
}


void GuiManager::update()
{
    m_gui.update();
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

void GuiManager::setupGuiEvents()
{
    m_gui.onDropdownEvent(this, &GuiManager::onDropdownEvent);
   // m_gui.onColorPickerEvent(this, &GuiManager::onColorPickerEvent);
    m_gui.onButtonEvent(this, &GuiManager::onButtonEvent);
    m_gui.onToggleEvent(this, &GuiManager::onToggleEvent);
    //m_gui.onMatrixEvent(this, &GuiManager::onMatrixEvent);
}


void GuiManager::saveGuiValues(string path)
{
    ofXml xml;
    ofSerialize(xml, m_parameters);
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
    ofDeserialize(xml, m_parameters);
}



void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    int margin =  LayoutManager::MARGIN;
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( m_gui.getPosition().x - margin, 0, m_gui.getWidth() + 2*margin, ofGetHeight());
    ofPopStyle();
}



void GuiManager::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "MODES")
    {
        AppManager::getInstance().getAudioManager().changeMode(e.child);
        m_gui.getDropdown(e.target->getName())->expand();
        m_gui.getDropdown(e.target->getName())->setLabel("MODE: " + ofToString(e.child));
    }
    
    else if(e.target->getName() == "SAMPLES")
    {
        AppManager::getInstance().getAudioManager().changeSample(e.child);
        m_gui.getDropdown(e.target->getName())->expand();
        m_gui.getDropdown(e.target->getName())->setLabel("SAMPLE: " + ofToString(e.child));
    }
    
}

void GuiManager::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getName() << " Selected" << endl;
    
}

void GuiManager::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getName() << " Selected" << endl;
    
}


void GuiManager::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getName() << " Selected" << endl;
    
}

void GuiManager::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
}

void GuiManager::setAudioMode(int value)
{
    if(value < 0 || value >= AudioManager::NUM_MODES){
        return;
    }
    
    string dropBoxName = "MODES";
    auto menu = m_gui.getDropdown(dropBoxName);
    menu->select(value);
    menu->setLabel("MODE:" + ofToString(value));
    AppManager::getInstance().getAudioManager().changeMode(value);
}

void GuiManager::setAudioIndex(int value)
{
    if(value < 0 || value >= AudioManager::NUM_SAMPLES){
        return;
    }
    
    string dropBoxName = "SAMPLES";
    auto menu = m_gui.getDropdown(dropBoxName);
    menu->select(value);
    menu->setLabel("SAMPLE:" + ofToString(value));
    AppManager::getInstance().getAudioManager().changeSample(value);
}

void GuiManager::setSerialConnected(bool value)
{
    m_gui.getToggle("Serial")->setChecked(value);
}


