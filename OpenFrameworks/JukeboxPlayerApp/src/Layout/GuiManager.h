/*
 *  GuiManager.h
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxGui.h"


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
    
    void setSerialConnected(bool value);

	void setRelayValue(bool value);

	void setLightValue(bool value);

	void setAudioMode(int value);

	void setAudioIndex(int value);
    
private:
    
    void setupGuiParameters();
    
    void drawGui();

    
    
    
private:
    
	ofxPanel             m_gui;
    
    ofParameter<float>    m_guiFPS;
    ofParameter<bool>     m_isSerial;
	ofParameter<bool>     m_relay;
	ofParameter<bool>     m_light;
    
    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


