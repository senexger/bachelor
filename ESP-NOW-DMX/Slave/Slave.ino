/*
   ESPNOW-SLAVE
   Date: 1th March 2020
   Author: Maximilian W. Gotthardt
   Purpose: DMX implementation via ESP-Now
*/

#include <esp_now.h>
#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>
#include "MacList.h"

#define CHANNELS_NEEDED 6

// TODO: Seems to bee wrong. Should also be spreaded via python bridge
#if DMX_BROADCASTING
  static uint8_t MAC_ADDRESS[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#else 
  static uint8_t MAC_ADDRESS[] = {0xFC, 0xF5, 0xC4, 0x31, 0x9A, 0x44};
#endif

// Variable to store if sending data was successful
String success;
String puropse = "SLAVE";

// Variables for sending and receiving
bool isDmxMetaReceived = 0;
uint8_t broadcastId;
uint8_t offset;

// UART checking
HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins

// ++++ STUFF FOR RECEIVE ++++

// Package including a ID to tell the package contaings meta or data and if data
typedef struct struct_esp_data_broadcast {
  uint8_t metaCode;
  uint8_t broadcastID;
  uint8_t payload[MAX_BROADCAST_FRAME_SIZE];
} struct_esp_data_broadcast;

// Package including just the ESP-Unicast information
typedef struct struct_esp_data_unicast {
  uint8_t payload[MAX_BROADCAST_FRAME_SIZE];
} struct_esp_data_unicast;

// new architecture 
typedef struct struct_advanced_meta {
  uint8_t metaCode;
  // information about broadcast
  uint8_t broadcastId;
  uint8_t broadcastOffset;
  // general information about the next test
  uint8_t slave_offset;
  uint8_t slave_broadcastId;
  uint8_t verbose;
  uint8_t debug;
  uint8_t timestamp;
  uint8_t airtime;
  uint16_t full_repetitions;
  uint8_t master_channel;
  uint8_t slave_channel;
  uint8_t dmx_broadcasting;
  uint16_t channel_total;
  uint8_t broadcast_frame_size;
  uint8_t unicast_frame_size;
  uint8_t unicast_slave_count;
  uint16_t send_repitition;
  uint16_t wait_after_send;
  uint16_t wait_after_rep_send;
} struct_advanced_meta;

// Create a struct_message called espBroadcastData
struct_esp_data_broadcast espBroadcastData;
struct_advanced_meta      advanced_Meta;
struct_esp_data_unicast   espUnicastData;
struct_advanced_meta      dmx_meta;

int thisBroadcastID;
int thisBroadcastOffset;
int correctCastCount = 0;

uint8_t successRatioArray[250];

void setup() {
  Serial.begin(115200);
  hSerial.begin(115200); // open Serial Port to the Master RX2 TX2 GND
  Serial.println("Slave Node here");

  Serial.println("init success!");

  setupEspNow();

  for (uint8_t i=0; i< 100; i++) {
    successRatioArray[i] = 42;
  }
}

void loop() {
  // wait for incoming messages
  delay(10000);
}

void sendResultsToMaster() {
  if(VERBOSE) Serial.println("Sending successRatioArray");
  
  addNodeToPeerlist(MASTER_MAC);

  esp_err_t unicastResult = esp_now_send(MASTER_MAC,
                                        (uint8_t *) &successRatioArray,
                                        SEND_REPITITION);
  if(DEBUG) espNowStatus(unicastResult);

  for (uint8_t i=0; i< 100; i++) {
    successRatioArray[i] = 0;
  }
  printSettings();
}

void setupEspNow() {
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_STA);
  if (VERBOSE) Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}