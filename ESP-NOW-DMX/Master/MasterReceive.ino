// // callback when data is recv from Master just printing incomming data
// void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
//   memcpy(&dmxData, incommingData, sizeof(dmxData));
//   Serial.print("Bytes received: "); Serial.print(data_len);
  
//   // magic number 20 should be data_len
//   bool signalBroken = false;
//   for (int i=0; i < data_len; i++) {
//     if (dmxData.payload[i] != i) {
//       signalBroken = true;
//     }
//   }
//   if (signalBroken) {
//     if(DEBUG) Serial.println(" (F)"); // F stands for False
//   }
//   else {
//     if(DEBUG) Serial.println(" (T)"); // T stands for True
//   }

//   // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
//   //          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//   // Serial.print("Last Packet Recv from: "); Serial.println(macStr);
// }