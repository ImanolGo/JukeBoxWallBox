/*
 *  AudioManager.cpp
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */



#include "ofMain.h"

#include "AudioManager.h"
#include "AppManager.h"


const float AudioManager::FADE_TIME_S = 1.0;
const int   AudioManager::NUM_MODES = 4;
const int   AudioManager::NUM_SAMPLES = 26;
const int   AudioManager::WAIT_TIME_MS = 4000;


AudioManager::AudioManager(): Manager(), m_currentSamplePath(""), m_currentMode(0), m_isPlaying(true), m_currentSample(0)
{
    //Intentionally left empty
}


AudioManager::~AudioManager()
{
    ofLogNotice() <<"AudioManager::Destructor" ;
}

void AudioManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    this->setupPlayer();
    this->setupSamples();
    this->setupTimers();
    this->setupText();
    
   
   
    ofLogNotice() <<"AudioManager::initialized" ;
    
}

void AudioManager::setupPlayer()
{
    m_audioVolume = ofPtr<BasicVisual> (new BasicVisual());
    m_audioVolume->setValue(0.0);
    
    m_soundPlayer.setLoop(false);
    m_soundPlayer.setVolume(0);
}


void AudioManager::setupSamples()
{
    
    ofLogNotice() <<"AudioManager::setupSamples " ;
    //some path, may be absolute or relative to bin/data
    string path = "audio/";
    ofDirectory dir(path);
    dir.listDir();
    dir.sort();
    
    ofLogNotice() <<"AudioManager::directory size: " << dir.size();
    
    
    for(int i = 0; i < dir.size(); i++)
    {
        string pathAux =  dir.getPath(i);
        ofDirectory dirAux(pathAux);
        dirAux.listDir();
        dirAux.sort();
        this->loadSubfolder(dirAux);
        
    }
}

void AudioManager::setupText()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth() - LayoutManager::MARGIN;
    float height = AppManager::getInstance().getSettingsManager().getAppHeight() - LayoutManager::MARGIN;
    
    float fontSize = 40;
    string text = "TEXT";
    string fontName =  LayoutManager::LAYOUT_FONT;
    ofPoint pos(width/2, height/2);
    m_text = TextVisual(pos,  width,  height, true);
    m_text.setText(text, fontName, fontSize, ofColor(210));
    m_text.drawBoundingBox(true);
    
//    m_text.init(fontName, fontSize);
//    m_text.setText(text);
//    m_text.wrapTextX(width);
//    m_text.setColor(255,255, 255,255);
    //m_text.wrapTextArea(width, height);
    
    ofLogNotice()<< "AudioManager::setupText";
    this->updateText();
    
    
}

bool AudioManager::loadSubfolder(ofDirectory& dir)
{
    ofLogNotice() <<"AudioManager::loading subfolders ..." ;
    //only show png files
    dir.allowExt("mp3");
    dir.allowExt("wav");
    dir.allowExt("wma");
    //populate the directory object
    
    
    if(dir.listDir()==0){
        ofLogNotice()<< "AudioManager::loadSubfolder-> No audio files found in: " << dir.getAbsolutePath();
        return false;
    }
    
    
    ofLogNotice()<< "AudioManager::loadSubfolder-> Path: " << dir.getAbsolutePath();
    ofLogNotice()<< "AudioManager::loadSubfolder-> Size: " << dir.size();
    
    SampleNameVector samples;
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        string name = dir.getPath(i);
        //ofLogNotice()<< "AudioManager::loadSubfolder-> Loaded: " << name;
        samples.push_back(name);
    }
    
    std::sort(samples.begin(), samples.end());
    
    for(int i = 0; i < samples.size(); i++){
        ofLogNotice()<< "AudioManager::loadSubfolder-> Loaded: " << samples[i];
    }
    
    
    m_sampleNames.push_back(samples);
    ofLogNotice()<< "AudioManager::loadSubfolder-> Samples Names Size: " << m_sampleNames.size();
    
    return true;
    
}




void AudioManager::setupTimers()
{
    auto time = AppManager::getInstance().getSettingsManager().getPressWaitingTime();
    m_timerSong.setup( time );
    //m_timer.start( false ) ;
    ofAddListener( m_timerSong.TIMER_COMPLETE , this, &AudioManager::timerCompleteHandlerSong ) ;
    
    ofLogNotice() <<"AudioManager::setupTimer << Time Song = : " << time << "ms";
    
    time = AppManager::getInstance().getSettingsManager().getRelayHighTime();
    m_timerRelay.setup( time );
    //m_timer.start( false ) ;
    ofAddListener( m_timerRelay.TIMER_COMPLETE , this, &AudioManager::timerCompleteHandlerRelay ) ;
    
    ofLogNotice() <<"AudioManager::setupTimer << Time Relay = : " << time << "ms";
}


void AudioManager::update()
{
    this->updateTimers();
    this->updatePlayer();
}

void AudioManager::updateText()
{
    //float width = AppManager::getInstance().getSettingsManager().getAppWidth() - LayoutManager::MARGIN;
    string text = "Mode: " + ofToString(m_currentMode) + ", Sample: " + ofToString(m_currentSample);
    
    
     m_text.setText(text);
     //m_text.wrapTextX(width);
    
}


void AudioManager::updatePlayer()
{
    m_soundPlayer.setVolume(m_audioVolume->getValue());
    
    if(m_isPlaying && !m_soundPlayer.isPlaying()){
        m_isPlaying = false;
        AppManager::getInstance().getSerialManager().sendSampleToggle(1);
        AppManager::getInstance().getSerialManager().sendRelayToggle(0);
        m_timerRelay.start(false,true);
    }
}

void AudioManager::updateTimers()
{
    m_timerSong.update();
    m_timerRelay.update();
}



void AudioManager::draw()
{
    this->drawText();
   // ofClear(255,0,0);
}

void AudioManager::drawText()
{
    m_text.draw();
    
    
//    float width = AppManager::getInstance().getSettingsManager().getAppWidth() - LayoutManager::MARGIN;
//    float height = AppManager::getInstance().getSettingsManager().getAppHeight() - LayoutManager::MARGIN;
//
//    m_text.drawCenter(width/2,height/2 - m_text.getHeight()*0.5);
//m_text.wrapTextX(width);
}


bool AudioManager::playSample(string path)
{
    if(!m_soundPlayer.load(path))
    {
        ofLogNotice() <<"AudioManager::playSample -> No sample found under path:  " << path ;
        return false;
    }
    else
    {
        
        m_currentSamplePath = path;
        //m_soundPlayer.setPosition(0);
        m_soundPlayer.setLoop(false); //Sound will not loop
        m_soundPlayer.play();
        ofLogNotice() <<"AudioManager::playSample ->  " << m_currentSamplePath;
  
    
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_audioVolume);

        EffectSettings settings; settings.animationTime = FADE_TIME_S;
        AppManager::getInstance().getVisualEffectsManager().createValueEffect(m_audioVolume, 1.0, settings);
        AppManager::getInstance().getSerialManager().sendSampleToggle(0);
        m_isPlaying = true;

        return true;
        
    }
    
    return false;
}

void AudioManager::stopSample()
{
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_audioVolume);

    EffectSettings settings; settings.animationTime = FADE_TIME_S;
    AppManager::getInstance().getVisualEffectsManager().createValueEffect(m_audioVolume, 0.0, settings);
    
    m_isPlaying = false;
    
    //m_soundPlayer.setPaused(true);
}


bool AudioManager::changeMode(int value)
{
    if(value<0 || value >= m_sampleNames.size()){
        return false;
    }
    
    m_currentMode = value;
    ofLogNotice() <<"AudioManager::changeMode -> Mode: " << m_currentMode;
    return true;
}

bool AudioManager::changeSample(int value)
{
   
    if(m_sampleNames.empty() || m_currentMode >= m_sampleNames.size())
    {
        ofLogNotice() <<"AudioManager::changeSample -> Error-> Sample Names Size: " << m_sampleNames.size() ;
        return false;
    }
    
    if(value<0 || value>= m_sampleNames[m_currentMode].size()){
        ofLogNotice() <<"AudioManager::changeSample -> Error-> Sample in Mode Size: " << m_sampleNames[m_currentMode].size() ;
        return false;
    }

    
    m_currentSample = value;
    m_currentSamplePath = m_sampleNames[m_currentMode][m_currentSample];

    ofLogNotice() <<"AudioManager::changeSample -> Mode: " << m_currentMode << ", Sample: " << m_currentSample ;
    ofLogNotice() <<"AudioManager::changeSample -> Path: " << m_currentSamplePath;
    
    m_timerSong.start(false,true);
    AppManager::getInstance().getSerialManager().sendSampleToggle(0);
    this->updateText();
    this->stopSample();
    
    return true;
}

void AudioManager::timerCompleteHandlerSong( int &args )
{
    ofLogNotice() <<"AudioManager::timerCompleteHandlerSong";
    this->playSample(m_currentSamplePath);
}


void AudioManager::timerCompleteHandlerRelay( int &args )
{
    ofLogNotice() <<"AudioManager::timerCompleteHandlerRelay";
    AppManager::getInstance().getSerialManager().sendRelayToggle(1);
    

}




