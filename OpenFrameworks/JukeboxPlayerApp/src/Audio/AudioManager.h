/*
 *  AudioManager.h
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */


#pragma once

#include "Manager.h"
#include "SettingsManager.h"
#include "BasicVisual.h"
#include "ofxSimpleTimer.h"
#include "TextVisual.h"
//#include "ofxTextSuite.h"


//========================== class AudioManager ==============================
//============================================================================
/** \class AudioManager AudioManager.h
 *	\brief Class managing the audio
 *	\details it loads the samples and gives the methods to play them
 */


class AudioManager: public Manager
{
    static const float    FADE_TIME_S; //< Fade time in seconds
    static const int    WAIT_TIME_MS; //< waiting time in ms
    
    
public:
    
     static const int    NUM_MODES; //< Number of audio modes
     static const int    NUM_SAMPLES; //< FNumber of audio samples per mode
    
    //! Constructor
    AudioManager();
    
    //! Destructor
    ~AudioManager();
    
    //! Setup the Audio Manager
    void setup();
    
    //! Update the Audio Manager
    void update();
    
    //! Draw the Audio Manager
    void draw();
    
    bool playSample(string path);
    
    void stopSample();
    
    bool changeMode(int value);
    
    bool changeSample(int value);
    
    void timerCompleteHandler( int &args ) ;
    
    
private:
    
    void setupPlayer();
    
    void setupText();
    
    void setupSamples();
    
    void setupTimer();
    
    void updateTimer();
    
    void updateText();
    
    void updatePlayer();
    
    void drawText();
    
    void loadSampleNames();
    
    void loadSubfolder(ofDirectory& dir);
    
private:
    
    typedef             vector<string>  SampleNameVector;
    typedef             vector<SampleNameVector> SampleNames;
   
    SampleNames         m_sampleNames;
    ofSoundPlayer       m_soundPlayer;
    string              m_currentSamplePath;
    ofPtr<BasicVisual>  m_audioVolume;
    
    ofxSimpleTimer      m_timer;
    
    int                 m_currentMode;
    int                 m_currentSample;
    bool                m_isPlaying;
    
    //ofxTextBlock          m_text;
    TextVisual          m_text;

};



