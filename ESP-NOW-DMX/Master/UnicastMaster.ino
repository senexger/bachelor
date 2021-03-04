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

uint8_t unicastData[MAX_UNICAST_FRAME_SIZE];

void setupUnicast() {
  Serial.println("Setup Unicast");
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

  for (int i=0; i <= MAX_UNICAST_FRAME_SIZE; i++) {
    unicastData[i] = i;
  }

  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  InitESPNow();

  // adding peers to the peerlist - allowing to send esp-now
  Serial.println("Adding Slaves to peerlist:");
  addNodeToPeerlist(SLAVE_MAC_1);
  addNodeToPeerlist(SLAVE_MAC_2);
  addNodeToPeerlist(SLAVE_MAC_3);
  addNodeToPeerlist(SLAVE_MAC_4);
  addNodeToPeerlist(SLAVE_MAC_5);

  if(DEBUG) esp_now_register_send_cb(onDataSent);
  // TODO usage for acknoledgements?
  // esp_now_register_recv_cb(onDataRecvUnicast);
}

// TODO what are you doing?! Cloning MAC for the setup function?
void copyArray(uint8_t array[6], uint8_t copy[6]) {
  for (int i=0; i <= 6; i++) {
    array[i] = copy[i];
  }
}

// send data
void sendDataEspUnicast() {
  // TODO: iterate over all MAC addresses und use sendUnicastToMac (sendUnicastBackToMac)
  if(VERBOSE) Serial.println("[Info] Init DMX Unicasting");

  for (int i = 0; i < slaveCount; i++) {
    if(VERBOSE) { 
      Serial.print("Unicast: "); Serial.println(i);
      for (int j=0; j < 6; j++) {
        Serial.print(unicastDataArray[i].mac[j]);Serial.print(":");
      }
      Serial.println("");
    }
    // ! if statement is JUST FOR TESTING! Faking to send to X nodes
    if (i == 0) {
      /*
      esp_err_t unicastResult = esp_now_send(BROADCAST_MAC,//unicastDataArray[i].mac, 
                                            // TODO place unicastData[] here
                                            // (uint8_t) &unicastData, //  ??!?
                                            (uint8_t *) &unicastDataArray[i].dmxFrame,
                                            sizeof(unicastDataArray[i].dmxFrame));
                                            */

    // ! This is just testing from the broadcast sendData function...
    esp_err_t unicastResult = esp_now_send(BROADCAST_MAC,
                                            (uint8_t *) &broadcastArray[i].dmxFrame,
                                            UNICAST_FRAME_SIZE);
      if(DEBUG) espNowStatus(unicastResult);
    }
    else {
      esp_err_t unicastResult = esp_now_send(unicastDataArray[1].mac, 
                                            (uint8_t *) &unicastDataArray[i].dmxFrame,
                                            sizeof(unicastDataArray[i].dmxFrame));
      if(DEBUG) espNowStatus(unicastResult);
    }

    delay(WAIT_AFTER_SEND); // No delay crashs the system
  }
}

// TODO Remove this function?
// // Split function to BroadcastMAster and UnicastMaster
// // callback when data is recv from Slave
// // /* NEEDED FOR SLAVE?!
// void onDataRecvUnicast(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
//   memcpy(&slave_information, incommingData, sizeof(slave_information));
//   if(DEBUG) { 
//     Serial.print("[OK] received from "); 
//     // print mac
//     for (int i = 0; i < 6; i++) {
//       Serial.printf("%02X", mac_addr[i]);
//       if (i < 5)Serial.print(":");
//     }
//     Serial.print(" ("); 
//     Serial.print(slave_information.channelCount); 
//     Serial.println(" Channel)");
//   }

//   // add peer to send the slave information
//   addNodeToPeerlist(mac_addr);
// }