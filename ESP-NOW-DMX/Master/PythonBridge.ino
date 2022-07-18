#include <ArduinoJson.h>
#include "MacList.h"

void pythonBridge () {

  Serial.println("Entering Python Bridge");

  String payload;	

  // Wait for Serial incommong from the pythonBridge
  while (!Serial.available()){}
  
  if (Serial.available())
	  payload = Serial.readStringUntil( '\n' );

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
	  Serial.println(error.c_str()); 
	  return;
  }
  
  // give json parameters to fake-constant-test-parameters
	VERBOSE              = doc["VERBOSE"];              
	DEBUG                = doc["DEBUG"];
	TIMESTAMP            = doc["TIMESTAMP"];        // taking timestamps
	AIRTIME              = doc["AIRTIME"];          // measuring airtime
	FULL_REPETITIONS     = doc["FULL_REPETITIONS"]; // how many times run the test
  MASTER_CHANNEL       = 7;
  SLAVE_CHANNEL        = 1;

  DMX_BROADCASTING     = doc["DMX_BROADCASTING"];     // 1 = Broadcast, 0 = Unicast
  RAPID_REPITITION     = doc["RAPID_REPITITION"];     // 1 // Times repeat a broadcast
  CHANNEL_TOTAL        = doc["CHANNEL_TOTAL"];        // 10 // Broadcast: total count of channels of all fixtures
  BROADCAST_FRAME_SIZE = doc["BROADCAST_FRAME_SIZE"]; // 10 // Broadcast: Channel/Broadcast
  UNICAST_FRAME_SIZE   = doc["UNICAST_FRAME_SIZE"];   // 20 // Unicast: Channel/Unicast
  SLAVE_COUNT          = doc["SLAVE_COUNT"];  // 4  // Unicast: Slave Count
  SEND_REPITITION      = doc["SEND_REPITITION"];      // 1  // Sending-Measuring Interval
  WAIT_AFTER_SEND      = doc["WAIT_AFTER_SEND"];      // 0  // delay between sendings - prevent errors?
  WAIT_AFTER_REP_SEND  = doc["WAIT_AFTER_REP_SEND"];  // 1000 // delay between sendings - prevent errors?

  if (VERBOSE) printSettings();
}