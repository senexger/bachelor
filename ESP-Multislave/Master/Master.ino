/* ESP32 als Master für eine ESP-Now verbindung
 *  Der ESP32 sucht nach einem WiFi Netzerk dessen Namen mit Slave beginnt
 *  Wenn er so ein Netzwerk gefunden hat merkt er sich die MAC-Adresse des
 *  Slaves und kann mit diesem über ESP_Now kommunizieren
 */

//Bibliotheken für Esp-Now und für WiFi einbinden
#include <esp_now.h>
#include <WiFi.h>

// Globale Kopie der Slave Information
// und ein Flag um zu wissen ob der Slave bereits gefunden wurde
bool slaveFound = 0;
esp_now_peer_info_t slave;

#define CHANNEL 3

//Datenstruktur für den Datenaustausch
struct DATEN_STRUKTUR {
    uint16_t temperatur = 0;
    uint32_t feuchte = 0;
};
 

// Init ESP Now mit Restart falls was schief geht
void InitESPNow() {
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    //Fehler also neuer Versuch
    ESP.restart();
  }
}

// Wir suchen einen Slave
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();
  // reset on each scan
  memset(&slave, 0, sizeof(slave));

  Serial.println("");
  if (scanResults == 0) {
    Serial.println("NoKein WiFi Gerät im AP Modus gefunden");
  } else {
    Serial.print(scanResults); Serial.println(" Geräte gefunden ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      // Prüfen ob der Netzname mit `Slave` beginnt
      if (SSID.indexOf("Slave") == 0) {
        // Ja, dann haben wir einen Slave gefunden
        Serial.println("Slave gefunden.");
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
        // MAC-Adresse aus der BSSID ses Slaves ermitteln und in der Slave info struktur speichern
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x%c",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = CHANNEL; // pick a channel
        slave.encrypt = 0; // no encryption

        slaveFound = 1;
        //Ohne dieses break könnte man auch mit mehr als einem Slave verbinden
        //Wenn keine Verschlüsselung verwendet wird sind bis zu 20 Slaves möglich
        break;
      }
    }
  }

  // clean up ram
  WiFi.scanDelete();
}

// Prüfe ob ein Slave bereits gepaart is
// Sonst wird der Slave mit dem Master gepaart
bool manageSlave() {
  if (slave.channel == CHANNEL) {
    const esp_now_peer_info_t *peer = &slave;
    const uint8_t *peer_addr = slave.peer_addr;
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(peer_addr);
    if ( !exists) {
      // Slave not paired, attempt pair
      Serial.print("Slave Status: ");
      esp_err_t addStatus = esp_now_add_peer(peer);
      if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Slave found to process");
    return false;
  }
}

// sende Daten an Slave
void sendData() {
 DATEN_STRUKTUR ed;  
    //Da wir die Daten in diesem Beispiel im Slave ignorieren
    //ist es gleichgültig, was wir senden                   
    ed.temperatur = 0;
    ed.feuchte = 0;
    //wir kopieren die Datenstruktur in einen Speicherblock
    uint8_t data[sizeof(ed)]; memcpy(data, &ed, sizeof(ed));
    const uint8_t *peer_addr = slave.peer_addr;
    //Wir senden die Daten und prüfen den Status
  Serial.print("Sending: ");
  esp_err_t result = esp_now_send(peer_addr, data, sizeof(data));
  Serial.print("Send Status: ");
  if (result == ESP_OK) {
    Serial.println("Success");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

// callback wenn die Daten zum Slave gesendet wurden
// hier sieht man wenn der Slave nicht erreichbar war
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


// callback wenn wir Daten vom Slave bekommen
void on_data_recv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  //MAC Adresse des Slaves zur Info
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  DATEN_STRUKTUR ed;
  //wir kopieren die Daten in die Datenstruktur
    memcpy(&ed, data, sizeof(ed));
    //und zeigen sie an
    Serial.print("Empfangen von "); Serial.println(macStr);
    Serial.print("Temperatur: "); Serial.print(ed.temperatur); Serial.println(" °C");
    Serial.print("Feuchte   : "); Serial.print(ed.feuchte); Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  //Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("ESPNow ESP32 als Master");
  // das ist die mac Adresse vom Master
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  //Wir registrieren die Callback Funktion am Ende des sendevorgangs
  esp_now_register_send_cb(on_data_sent);
  //Wir registrieren die Callback Funktion für den Empfang
  esp_now_register_recv_cb(on_data_recv);
}

void loop() {
  // Wenn wir noch keinen Slave gefunden haben suchen wir weiter
  if (!slaveFound) ScanForSlave();
  if (slaveFound) {
    //haben wir einen Slave muss er gepaart werden
    //falls das noch nicht geschehen ist
    bool isPaired = manageSlave();
    if (isPaired) {
      // Wenn Master und Slave gepaart sind, können wir senden
      sendData();
    } else {
      // slave pair failed
      Serial.println("Slave pair failed!");
    }
  }
  else {
    // No slave found to process
  }

  // 5 Sekunden warten
  delay(5000);
}