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
    
    void sendSampleToggle(bool value);
    
    void sendRelayToggle(bool value);
    
    bool getConnected() const {return m_connected;}
    
    void onNewMessage(string & message);
    
private:
    
    void readSerialSettings();
    
    void setupSerial();
    
    void autoConnect();
    
    void connect(int portNum);
    
    bool checkConnection(int portNum);
    
    void sendConnection();
    
    bool receivedConnected();
    
    bool isData(const string & message);
    
    bool isConnected(const string & message);
    
    bool parseData(const string & message);

    void printHex(unsigned char * buffer, int size);

    void writeString(string message);
    
private:
    
    ofSerial	  m_serial;
    //ofxSimpleSerial	  m_serial;
    bool                m_connected;
    string			    m_messageBuffer;
	unsigned char	    m_bytesReturned[NUM_BYTES];
    
    
};


