/*
 *  SerialManager.cpp
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */



#include "SerialManager.h"
#include "AppManager.h"


const int SerialManager::BAUD_RATE = 115200;


SerialManager::SerialManager(): Manager(), m_connected(false)
{
    //Intentionally left empty
}

SerialManager::~SerialManager()
{
    ofLogNotice() << "SerialManager::destructor";
    m_serial.close();
}


//--------------------------------------------------------------

void SerialManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupSerial();
    this->sendSampleToggle(1);
    this->sendRelayToggle(0);
    
    ofLogNotice() <<"SerialManager::initialized" ;
}


void SerialManager::setupSerial()
{
   // ofLogNotice() <<"SerialManager::setupSerial << Autoconnecting serial port";
    //this->autoConnect();
    
    int serialPort = AppManager::getInstance().getSettingsManager().getSerialPort();
    
    ofLogNotice() <<"SerialManager::setupSerial <<  Serial Port from settings: " << serialPort;
    
    if(serialPort<0){
        ofLogNotice() <<"SerialManager::setupSerial << Autoconnecting serial port";
        this->autoConnect();
    }
    else{
        ofLogNotice() <<"SerialManager::setupSerial << Connecting serial to port " << serialPort;
        this->connect(serialPort);
    }
}

void SerialManager::connect(int portNum)
{
    if(m_serial.setup(portNum, BAUD_RATE)) //open a device number
    {
        ofLogNotice() <<"SerialManager::connect << Arduino connected to " << portNum;
        m_connected = true;
    }
    else{
         m_connected = false;
    }
    
    AppManager::getInstance().getGuiManager().setSerialConnected(m_connected);
}

void SerialManager::autoConnect()
{
    m_serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = m_serial.getDeviceList();
    
    // this should be set to whatever com port your serial device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
    // arduino users check in arduino app....
    
    m_connected = false;
    
    for(auto device: deviceList)
    {
        if(this->checkConnection(device.getDeviceID())) //open a device number
        {
            ofLogNotice() <<"SerialManager::setupSerial << Arduino connected to port " << device.getDeviceName();
            m_connected = true;
            return;
        }
    }
    
    AppManager::getInstance().getGuiManager().setSerialConnected(m_connected);
}


bool SerialManager::checkConnection(int portNum)
{
    if(m_serial.setup(portNum, BAUD_RATE)) //open a device number
    {
        //m_serial.flush();
        this->sendConnection();
        ofSleepMillis(500);
        if(this->receivedConnected()){
            ofLogNotice() <<"SerialManager::checkConnection << Arduino connected to " << portNum;
            return true;
        }
        else {
            m_serial.close();
        }
    }
    
    ofLogNotice() <<"SerialManager::checkConnection << Arduino is not connected ";
    //m_serial.setup(0, BAUD_RATE);
    return false;
    
}

void SerialManager::sendConnection()
{
    bool connected = 0;

    string message = "";
    message+= "c,";
    message+= ofToString(connected); message+= '|';

    ofLogNotice() <<"SerialManager::sendConnection << Sent: " << message;
    m_serial.writeBytes((unsigned char *) message.c_str(), message.length());
}

bool SerialManager::receivedConnected()
{
    ofLogNotice() <<"SerialManager::checkConnection << Serial Available: " << m_serial.available();
    
     string message = "";

    // if we've got new bytes
	if ( m_serial.available() > 0)
	{
		// we will keep reading until nothing is left
		while ( m_serial.available() > 0)
		{
			// we'll put the incoming bytes into bytesReturned
			 m_serial.readBytes(m_bytesReturned,  NUM_BYTES);

			// if we find the splitter we put all the buffered messages
			//   in the final message, stop listening for more data and
			//   notify a possible listener
			// else we just keep filling the buffer with incoming bytes.
			if (*m_bytesReturned == '\n')
			{
				message = m_messageBuffer;
				m_messageBuffer = "";
                // clear the message buffer
		        memset(m_bytesReturned, 0, NUM_BYTES);
                ofLogNotice() <<"SerialManager::receivedConnected -> NewMessage :  " << message;
                return  this->isConnected(message);
			}
			else
			{
                m_messageBuffer += *m_bytesReturned;
			}
			//cout << "  messageBuffer: " << messageBuffer << "\n";
		}

	}


    // clear the message buffer
    memset(m_bytesReturned, 0, NUM_BYTES);
    return false;
}



bool SerialManager::isConnected(const string & message)
{

    vector<string> input = ofSplitString(message, ",");
    
    if(input.size()>0 && input.front() == "c")
    {
        ofLogNotice() <<"SerialManager::isConnected -> TRUE ";
        return true;
    }
    
    ofLogNotice() <<"SerialManager::isConnected -> FALSE ";
    return false;
}

bool SerialManager::isData(const string & message)
{
   vector<string> input = ofSplitString(message, ",");
    
    if(input.size()>0 && input.front() == "d")
    {
        ofLogNotice() <<"SerialManager::isData -> TRUE ";
        return true;
    }
    
    
    ofLogNotice() <<"SerialManager::isData -> FALSE ";
    return false;
}

bool SerialManager::parseData(const string & message)
{

    vector<string> input = ofSplitString(message, ",");

      if(!this->isData(message)){
        return false;
    }
    
    if(input.size()<3)
    {
        ofLogNotice() <<"SerialManager::parseData -> data size too small -> " << input.size() ;
        return false;
    }
    
    int audioMode = ofToInt(input[2]);
    int audioIndex= ofToInt(input[1]);
    AppManager::getInstance().getGuiManager().setAudioMode(audioMode);
    AppManager::getInstance().getGuiManager().setAudioIndex(audioIndex);

    return true;
}



void SerialManager::update()
{
    if(!m_connected){
        return;
    }

      string message = "";

    // if we've got new bytes
	if ( m_serial.available() > 0)
	{
		// we will keep reading until nothing is left
		while ( m_serial.available() > 0)
		{
			// we'll put the incoming bytes into bytesReturned
			 m_serial.readBytes(m_bytesReturned,  NUM_BYTES);

			// if we find the splitter we put all the buffered messages
			//   in the final message, stop listening for more data and
			//   notify a possible listener
			// else we just keep filling the buffer with incoming bytes.
			if (*m_bytesReturned == '\n')
			{
				message = m_messageBuffer;
				m_messageBuffer = "";
                // clear the message buffer
		        memset(m_bytesReturned, 0, NUM_BYTES);

                ofLogNotice() <<"SerialManager::update -> NewMessage :  " << message;
                this->parseData(message);
                break;
			}
			else
			{
                m_messageBuffer += *m_bytesReturned;
			}
			//cout << "  messageBuffer: " << messageBuffer << "\n";
		}

	}


    // clear the message buffer
    memset(m_bytesReturned, 0, NUM_BYTES);

}


void SerialManager::onNewMessage(string & message)
{
    ofLogNotice() <<"SerialManager::onNewMessage ->  " << message;
	this->parseData(message);
}

void SerialManager::sendSampleToggle(bool value)
{
    if(!m_connected){
        return;
    }

    ofLogNotice() <<"SerialManager::sendSampleToggle ->  " << value;
    
    int channel = 0;

    string message = "";
    message+= "d,";
    message+= ofToString(channel); message+= ",";
    message+= ofToString(value); message+= '|';

    ofLogNotice() <<"SerialManager::sendSampleToggle ->  message: " << message;

    this->writeString(message);
}

void SerialManager::sendRelayToggle(bool value)
{
    if(!m_connected){
        return;
    }
    
    
    ofLogNotice() <<"SerialManager::sendRelayToggle ->  " << value;

    int channel = 2;

    string message = "";
    message+= "d,";
    message+= ofToString(channel); message+= ",";
    message+= ofToString(value); message+= '|';

    ofLogNotice() <<"SerialManager::sendRelayToggle ->  message: " << message;


    this->writeString(message);

}


void SerialManager::printHex(unsigned char * buffer, int size)
{
    std::stringstream ss;
    for(int i=0; i<size; ++i){
        ss << std::hex << (int)buffer[i] << " ";
    }
    std::string mystr = ss.str();
    
    ofLogNotice() <<"SerialManager::SerialManager ->  hex: " << mystr;
}

void SerialManager::writeString(string message)
{
	unsigned char* chars = (unsigned char*) message.c_str(); // cast from string to unsigned char*
	int length = message.length();
	m_serial.writeBytes(chars, length);
}
