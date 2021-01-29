/*
// Broadcast mac 
// for broadcasts the addr needs to be ff:ff:ff:ff:ff:ff
// all devices on the same channel

#ifndef MACLIST_H
#define MACLIST_H

    static uint8_t BROADCAST_MAC[]      = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // M
    static uint8_t SLAVE_MAC_1[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C }; // I
    // static uint8_t SLAVE_MAC_2[6]       = { 0x24, 0x0A, 0xC4, 0x61, 0x19, 0x08 }; // II
    static uint8_t SLAVE_MAC_2[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C }; // III
    static uint8_t SLAVE_MAC_3[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_4[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_5[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_6[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_7[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_8[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_9[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    // static uint8_t SLAVE_MAC_2[6]       = { 0xFC, 0xF5, 0xC4, 0x32, 0x69, 0x0d };
    // static uint8_t SLAVE_MAC_3[6]       = { 0xFC, 0xF5, 0xC4, 0x33, 0x69, 0x0d };
    // static uint8_t SLAVE_MAC_4[6]       = { 0xFC, 0xF5, 0xC4, 0x34, 0x6a, 0x0e };
    // static uint8_t SLAVE_MAC_5[6]       = { 0xFC, 0xF5, 0xC4, 0x35, 0x6d, 0x0C };
    // static uint8_t SLAVE_MAC_6[6]       = { 0xFC, 0xF5, 0xC4, 0x39, 0x6b, 0x0C };
    // static uint8_t SLAVE_MAC_7[6]       = { 0xFC, 0xF5, 0xC4, 0x36, 0x6b, 0x0C };
    // static uint8_t SLAVE_MAC_8[6]       = { 0xFC, 0xF5, 0xC4, 0x34, 0x6a, 0x0C };
    // static uint8_t SLAVE_MAC_9[6]       = { 0xFC, 0xF5, 0xC4, 0x33, 0x6f, 0x0C };

    static uint8_t slave_offset         = 30;
    static uint8_t slave_broadcastId    = 1;

#endif

// General Constants
// print level
// TODO SLAVE_CHANNEL
#define MASTER_CHANNEL       7
#define SLAVE_CHANNEL        1

// MASTER CONSTANTS:
#define DMX_BROADCASTING     0    // 1 = Broadcast, 0 = Unicast
#define CHANNEL_TOTAL        10   // Broadcast: total count of channels of all fixtures
#define BROADCAST_FRAME_SIZE 10   // Broadcast: Channel/Broadcast
#define UNICAST_FRAME_SIZE   20   // Unicast: Channel/Unicast
#define SEND_REPITITION      1    // Sending-Measuring Interval
#define WAIT_AFTER_SEND      0    // delay between sendings - prevent errors?
#define WAIT_AFTER_REP_SEND  1000 // delay between sendings - prevent errors?

// Global copy of slave
#define MAX_SLAVES           20
*/

#include <WiFi.h>
#include "ArduinoJson.h"

// +++ VARIABLES FOR TESTING +++
int VERBOSE = 1;              
int DEBUG = 1;
int TIMESTAMP = 1;            // taking timestamps
int AIRTIME = 0;              // measuring airtime

int FULL_REPETITIONS = 100;     // how many times run the test

void pythonBridge () {

  int status = 0;

  int size_ = 0;
  String payload;	

  while ( !Serial.available()  ){
	// Serial.println("here");
  }
  
  if ( Serial.available() )
	payload = Serial.readStringUntil( '\n' );
  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
	Serial.println(error.c_str()); 
	return;
  }

  Serial.println("Set Variables:");
  
  // TODO give variables to fakeconstants
	VERBOSE          = doc["VERBOSE"] ;              
	DEBUG            = doc["DEBUG"];
	TIMESTAMP        = doc["TIMESTAMP"];        // taking timestamps
	AIRTIME          = doc["AIRTIME"];          // measuring airtime
  
  Serial.print("AIRTIME: "); Serial.println(AIRTIME);
	
	int FULL_REPETITIONS = doc["FULL_REPETITIONS"]; // how many times run the test

  // if (doc["VERBOSE"] == 1) {
  if (AIRTIME == 0) {
	 Serial.println("{\"Success\":\"True\"}");
	 status = 1;
  }
  else {
	  Serial.println("{\"Success\":\"False\"}");
	  status = -1;
   }
}