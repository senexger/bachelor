/* All unicast specific functions */

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
}

// send data
void sendESPUnicast() {
  // TODO: iterate over all MAC addresses und use sendUnicastToMac
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
