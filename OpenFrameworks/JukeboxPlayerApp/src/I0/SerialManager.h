/*
 *  SerialManager.h
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */



#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxSimpleTimer.h"

#define NUM_BYTES 1

//#include "ofxSimpleSerial.h"


//========================== class SerialManager =======================================
//==============================================================================
/** \class SerialManager SerialManager.h
 *    \brief class for managing the serial connection
 *    \details  It connects the Adafruit Feather M0 board to communicate color changes on the RGB LEDs.
 */


class SerialManager: public Manager
{
    static const int BAUD_RATE; 
    
public:
    //! Constructor
    SerialManager();
    
    //! Destructor
    virtual ~SerialManager();
    
    //! setups the udp manager
    void setup();
    
    //! updates the udp manager
    void update();
    
    //void onSetColor(const ofColor& color);
    
    void sendLightToggle(bool & value);
    
    void sendRelayToggle(bool & value);
    
    bool getSerialConnected() const {return m_serialConnected;}

	bool getLoraConnected() const { return m_loraConnected; }
    
    void onNewMessage(string & message);
    
private:
    
    void readSerialSettings();
    
    void setupSerial();

	void setupTimer();
    
    void autoConnect();
    
    void connect(int portNum);
    
    bool checkConnection(int portNum);
    
    void sendConnection();
    
    bool receivedConnected();
    
    bool isData(const string & message);
    
    bool isConnected(const string & message);

	bool isLoraConnected(const string & message);

	void setLoraConnected();
    
    bool parseData(const string & message);

    void printHex(unsigned char * buffer, int size);

    void writeString(string message);

	void timerCompleteHandler(int &args);
    
private:
    
    ofSerial			 m_serial;
	ofxSimpleTimer       m_timerLora;
    //ofxSimpleSerial	  m_serial;
    bool                m_serialConnected;
	bool                m_loraConnected;
    string			    m_messageBuffer;
	unsigned char	    m_bytesReturned[NUM_BYTES];
    
    
};


