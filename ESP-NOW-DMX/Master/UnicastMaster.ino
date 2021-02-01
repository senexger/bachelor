#include "MacList.h"

/* All unicast specific functions */

// Setup dmx unicast messages
struct_dmx_unicast unicastData1;
struct_dmx_unicast unicastData2;
struct_dmx_unicast unicastData3;
struct_dmx_unicast unicastData4;
struct_dmx_unicast unicastData5;
struct_dmx_unicast unicastData6;
struct_dmx_unicast unicastData7;
struct_dmx_unicast unicastData8;
struct_dmx_unicast unicastData9;
struct_dmx_unicast unicastDataArray[9];


void setupUnicast() {
  // Create Data
  for (int i=0; i<=MAX_SLAVES; i++) { unicastData1.dmxFrame[i] = i; }
  copyArray(unicastData1.mac, SLAVE_MAC_1);
  unicastData1.mac[6] = SLAVE_MAC_1[6]; // why?
  unicastDataArray[0] = unicastData1;
  for (int i=0; i<=MAX_SLAVES; i++) { unicastData2.dmxFrame[i] = i; }
  copyArray(unicastData2.mac, SLAVE_MAC_2);
  unicastDataArray[1] = unicastData2;
  for (int i=0; i<=MAX_SLAVES; i++) { unicastData3.dmxFrame[i] = i; }
  copyArray(unicastData3.mac, SLAVE_MAC_3);
  unicastDataArray[2] = unicastData3;

  // Prepare ESP-NOW
  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  InitESPNow();

  InitESPTimer();

  esp_now_register_recv_cb(onDataRecvUnicast);
}

void copyArray(uint8_t array[6], uint8_t copy[6]) {
  for (int i=0; i <= 6; i++) {
    array[i] = copy[i];
  }
}

// send data
void sendESPUnicast() {
  // TODO: iterate over all MAC addresses und use sendUnicastToMac (sendUnicastBackToMac)
  if(VERBOSE) Serial.println("[Info] Init DMX Unicasting");
  for (int i = 0; i < slaveCount; i++) {
    if(VERBOSE) { 
      Serial.print("Unicast: "); Serial.println(i);
      for (int j=0; j < 6; j++) {
        Serial.print(unicastDataArray[i].mac[j]);Serial.print(":");
      }
    }
    // if statement is JUST FOR TESTING! Faking to send to X nodes
    if (i == 0) {
      esp_err_t unicastResult = esp_now_send(unicastDataArray[i].mac, 
                                            (uint8_t *) &unicastDataArray[i].dmxFrame,
                                            sizeof(unicastDataArray[i].dmxFrame));
      if(DEBUG) espNowStatus(unicastResult);
    }
    else {
      esp_err_t unicastResult = esp_now_send(unicastDataArray[1].mac, 
                                            (uint8_t *) &unicastDataArray[i].dmxFrame,
                                            sizeof(unicastDataArray[i].dmxFrame));
      if(DEBUG) espNowStatus(unicastResult);
    }

    // sendUnicastStupid(unicastDataArray[i].mac, unicastDataArray[i].dmxFrame);
    delay(WAIT_AFTER_SEND); // No delay crashs the system
  }
}

// Split function to BroadcastMAster and UnicastMaster
// callback when data is recv from Slave
// /* NEEDED FOR SLAVE?!
void onDataRecvUnicast(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  memcpy(&slave_information, incommingData, sizeof(slave_information));
  if(DEBUG) { 
    Serial.print("[OK] received from "); 
    // print mac
    for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", mac_addr[i]);
      if (i < 5)Serial.print(":");
    }
    Serial.print(" ("); 
    Serial.print(slave_information.channelCount); 
    Serial.println(" Channel)");
  }

  // add peer to send the slave information
  addNodeToPeerlist(mac_addr);
}