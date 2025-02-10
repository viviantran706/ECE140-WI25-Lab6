#include "ECE140_WIFI.h"
#include "ECE140_MQTT.h"

// MQTT client - using descriptive client ID and topic
#define CLIENT_ID "esp32-sensors"
#define TOPIC_PREFIX "ECE140a/ece140/sensors"

ECE140_MQTT mqtt(CLIENT_ID, TOPIC_PREFIX);

// WiFi credentials
const char* ucsdUsername = UCSD_USERNAME;
const char* ucsdPassword = UCSD_PASSWORD;
const char* wifiSsid = WIFI_SSID;
const char* nonEnterpriseWifiPassword = NON_ENTERPRISE_WIFI_PASSWORD;

void setup() {
    Serial.begin(115200);
    ECE140_WIFI wifi;
    wifi.connectToWiFi(wifiSsid, NON_ENTERPRISE_WIFI_PASSWORD);

    if(mqtt.connectToBroker()){
        mqtt.publishMessage("status", "Sensor device Connected");
        Serial.println("Connect Tp Mqqt Broker");
    } else{
        Serial.println("Failed to connect to MQTT broker");
    }
}

void loop() {
    mqtt.loop();
    int hallValue = hallRead();

    float temperature = temperatureRead();
    String message = "{(\"hall_sensor\": " + String(hallValue) + 
)}"

}