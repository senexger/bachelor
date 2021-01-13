// #define PRINTSCANRESULTS 0

// // Scan for slaves in AP mode
// void ScanForSlave() {
//   int8_t scanResults = WiFi.scanNetworks();
//   //reset slaves
//   memset(slaves, 0, sizeof(slaves));
//   slaveCount = 0;
//   Serial.println("");
//   if (scanResults == 0) {
//     Serial.println("No WiFi devices in AP Mode found");
//   } else {
//     Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
//     for (int i = 0; i < scanResults; ++i) {
//       // Print SSID and RSSI for each device found
//       String SSID = WiFi.SSID(i);
//       int32_t RSSI = WiFi.RSSI(i);
//       String BSSIDstr = WiFi.BSSIDstr(i);

//       if (PRINTSCANRESULTS) {
//         Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); 
//         Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
//       }
//       delay(10);
//       // Check if the current device starts with `Slave`
//       if (SSID.indexOf("Slave") == 0) {
//         // SSID of interest
//         Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); 
//         Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
//         // Get BSSID => Mac Address of the Slave
//         int mac[6];

//         if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
//           for (int ii = 0; ii < 6; ++ii ) {
//             slaves[slaveCount].peer_addr[ii] = (uint8_t) mac[ii];
//           }
//         }
//         slaves[slaveCount].channel = MASTER_CHANNEL; // pick a channel
//         slaves[slaveCount].encrypt = 0; // no encryption
//         slaveCount++;
//       }
//     }
//   }
//   if (slaveCount > 0) {
//     Serial.print(slaveCount); Serial.println(" Slave(s) found, processing..");
//   } else {
//     Serial.println("No Slave Found, trying again.");
//   }

//   // clean up ram
//   WiFi.scanDelete();
// }

// // Check if the slave is already paired with the master.
// // If not, pair the slave with master
// void manageSlave() {
//   if (slaveCount > 0) {
//     for (int i = 0; i < slaveCount; i++) {
//       Serial.print("Processing: ");
//       for (int ii = 0; ii < 6; ++ii ) {
//         Serial.print((uint8_t) slaves[i].peer_addr[ii], HEX);
//         if (ii != 5) Serial.print(":");
//       }
//       Serial.print(" Status: ");
//       // check if the peer exists
//       bool exists = esp_now_is_peer_exist(slaves[i].peer_addr);
//       if (exists) {
//         // Slave already paired.
//         Serial.println("Already Paired");
//       } else {
//         // Slave not paired, attempt pair
//         esp_err_t addStatus = esp_now_add_peer(&slaves[i]);
//         if(DEBUG) espNowStatus(addStatus);
//         delay(100);
//       }
//     }
//   } else {
//     // No slave found to process
//     Serial.println("No Slave found to process");
//   }
// }