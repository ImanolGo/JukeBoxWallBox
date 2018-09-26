#pragma once


/*************************************************************************
 Protocol
 *************************************************************************/

#define HEADER_SIZE 5
#define SIZE_INDEX 3
#define COMMAND_INDEX 4
#define DATA_BYTES 2


/*************************************************************************
 LoRa
 *************************************************************************/

//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0
