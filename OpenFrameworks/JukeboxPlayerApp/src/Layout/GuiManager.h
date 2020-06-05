/*
 *  GuiManager.h
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxGuiExtended.h"


//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *    \brief Class managing the application´s grapical user interface (GUI)
 *    \details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    static const float GUI_WIDTH;
    
public:
    
    //! Constructor
    GuiManager();
    
    //! Destructor
    ~GuiManager();
    
    //! Set-up the gui
    void setup();
    
    //! update the gui
    void update();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues(string path = "");
    
    void loadGuiValues(string path = "");
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return GUI_WIDTH;}
    
    void setAudioMode(int value);
    
    void setAudioIndex(int value);
    
    void setSerialConnected(bool value);

	void setRelayValue(bool value);

	void setLightValue(bool value);
    
    void setIndex(int& index);
    
    void setMode(int& index);
    
private:
    
    void setupGuiParameters();
    
    void setupModesGui();
    
    void setupAudioGui();
    
    void drawRectangle();
    
    void drawGui();

    
    
    
private:
    
    ofxGui             m_gui;
    
    ofParameter<float>    m_guiFPS;
    ofParameter<bool>     m_isSerial;
	ofParameter<bool>     m_relay;
	ofParameter<bool>     m_light;
    
    vector<ofParameter<bool>> m_modeVector;
    vector<ofParameter<bool>> m_indexVector;
    
    //mode panel
    ofxGuiPanel*  m_modePanel;
    ofxGuiGroup*  m_modeToggles;
    
    //sample panel
    ofxGuiPanel* m_indexPanel;
    ofxGuiGroup* m_indexToggles;
    
    //color panel
    ofxGuiPanel* colorPanel;
    ofxGuiGroup *colorToggles;
    
    ofParameterGroup        m_modes;
    ofParameterGroup        m_indexes;
    vector<ofColor>         m_colors;
    int                     m_switchColor;
    
    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


