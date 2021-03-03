#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>

void setupSlaveBroadcasting() {
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecvBroadcast);
}

// callback when data is recv from Master just printing incomming data
// TODO devide in Broadcast & Unicast
void OnDataRecvBroadcast(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  if(VERBOSE) Serial.println("VERBOSE: OnDataRecvBroadcast()");
  // TODO ist das sinnvoll?!
  if (AIRTIME) {
    Serial2.print("!");
  }
  Serial.print("Data len: "); Serial.println(data_len);

  // META PACKAGE HANDLING
  if (incommingData[0]) { // == advanced_Meta.metaCode from master struct
    applyMetaInformation(incommingData, data_len);
  }
  // DATA PACKAGE HANDLING
  else {
    if (checkPayload(incommingData, data_len)) {
      if(DEBUG) {
        Serial.print("[ERROR] Incomming Data broken: "); 
        Serial.print(data_len);
        Serial.println(" B");
      }
    }
    else {
      if(DEBUG) { 
        Serial.print("[OK] Rcvd: "); 
        Serial.print(data_len);
        Serial.println(" B");
       // TODO: timestamp here...
      }
    }
    // TODO: sure?
    getTimestamp();
    setTimestamp();
  }
}

// TODO: Can this be removed?!
// void sendESPCast(uint8_t mac_address[6]) {
//   // TODO why should I not unicast?
//   // * unicast: einfacher, aber wenn der Master geändert wird, müssen alle Slave benachrichtigt werden
//   // * broadcast: wie können die Slaves erkennen der Broadcast keine wichtigen Daten enthält?!
//   if(VERBOSE && DMX_BROADCASTING)  Serial.println("[Info] Begin BROADCAST");
//   if(VERBOSE && !DMX_BROADCASTING) Serial.println("[Info] Begin UNICAST");
//   esp_err_t broadcastResult = esp_now_send(mac_address,
//                                           (uint8_t *) &slaveRequirements,
//                                           sizeof(slaveRequirements));
//   espNowStatus(broadcastResult);
// }

// Callback when data is sent - Slave
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   // Serial.println("Last Packet Send Status");
//   if (status != ESP_NOW_SEND_SUCCESS) {
//     Serial.println("[ERROR] Delivery Fail");
//   }
//   else {
//     Serial.println("[OK] Send Ack");
//   }
// }

void applyMetaInformation(const uint8_t *incommingData, int data_len) {
  Serial.println("THIS IS META!!!");
  memcpy(&advanced_Meta, incommingData, sizeof(advanced_Meta));

  // APPLY SETTINGS
  VERBOSE              = advanced_Meta.verbose;
  DEBUG                = advanced_Meta.debug;
  TIMESTAMP            = advanced_Meta.timestamp;
  AIRTIME              = advanced_Meta.airtime;
  FULL_REPETITIONS     = advanced_Meta.full_repetitions;
  MASTER_CHANNEL       = advanced_Meta.master_channel;
  SLAVE_CHANNEL        = advanced_Meta.slave_channel;
  DMX_BROADCASTING     = advanced_Meta.dmx_broadcasting;
  CHANNEL_TOTAL        = advanced_Meta.channel_total;
  BROADCAST_FRAME_SIZE = advanced_Meta.broadcast_frame_size;
  UNICAST_FRAME_SIZE   = advanced_Meta.unicast_frame_size;
  SEND_REPITITION      = advanced_Meta.send_repitition;
  WAIT_AFTER_SEND      = advanced_Meta.wait_after_send;
  WAIT_AFTER_REP_SEND  = advanced_Meta.wait_after_rep_send;
  // broadcast settings
  broadcastId          = advanced_Meta.broadcastId;
  offset               = advanced_Meta.broadcastOffset;
  slave_offset         = advanced_Meta.slave_offset;
  slave_broadcastId    = advanced_Meta.slave_broadcastId;

  // PRINT SETTINGS
  printSettings();
  // broadcast specific information
  Serial.print("BroadcastID      : "); Serial.println(advanced_Meta.broadcastId);
  Serial.print("BroadcastOffset  : "); Serial.println(advanced_Meta.broadcastOffset);
  // generell information
  Serial.print("SlaveOffset      : "); Serial.println(advanced_Meta.slave_offset);
  Serial.print("SlaveBroadcastID : "); Serial.println(advanced_Meta.slave_broadcastId);
}

int checkPayload(const uint8_t *incommingData, int data_len ) {
  memcpy(&espBroadcastData, incommingData, sizeof(data_len));
  if (VERBOSE) {
    Serial.print("espBroadcastData.broadcastID=");Serial.println(espBroadcastData.broadcastID);
    Serial.print("espBroadcastData.payload[0] =");Serial.println(espBroadcastData.payload[0]);
    Serial.print("espBroadcastData.payload[1] =");Serial.println(espBroadcastData.payload[1]);
    Serial.print("espBroadcastData.payload[2] =");Serial.println(espBroadcastData.payload[2]);
    Serial.print("espBroadcastData.payload[3] =");Serial.println(espBroadcastData.payload[3]);
    Serial.print("espBroadcastData.payload[4] =");Serial.println(espBroadcastData.payload[4]);
    Serial.print("espBroadcastData.payload[5] =");Serial.println(espBroadcastData.payload[5]);
  }

  // check broadcast integrity
  // sub 1 becaus there is no broadcastID in payload
  // iterating through the payload
  bool signalHealthy = true;
  if (VERBOSE && espBroadcastData.broadcastID == broadcastId) {
    for (int i=1; i < data_len -1; i++) { 
      // just select needed channel
      // if ((i >= offset) && (i < offset + CHANNELS_NEEDED)) {
        Serial.print(i); 
        Serial.print(" = "); 
        Serial.println(espBroadcastData.payload[i]); 
      // }
      if (espBroadcastData.payload[i] != i) {
        signalHealthy = false;
      }
    }
  }
  return signalHealthy;
}