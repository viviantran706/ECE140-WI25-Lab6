#include "ECE140_WIFI.h"
#include "ECE140_MQTT.h"

// MQTT client - using descriptive client ID and topic
#define CLIENT_ID "esp32-sensors"
#define TOPIC_PREFIX "ENTER_SOMETHING_UNIQUE_HERE_THAT_SHOULD_ALSO_MATCH_MAINPY/ece140/sensors"

ECE140_MQTT mqtt(CLIENT_ID, TOPIC_PREFIX);

// WiFi credentials
const char* ucsdUsername = UCSD_USERNAME;
const char* ucsdPassword = UCSD_PASSWORD;
const char* wifiSsid = WIFI_SSID;
const char* nonEnterpriseWifiPassword = NON_ENTERPRISE_WIFI_PASSWORD;

void setup() {
    Serial.begin(115200);
}

void loop() {

}