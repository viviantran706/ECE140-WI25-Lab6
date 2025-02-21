#include "ECE140_WIFI.h"
#include "ECE140_MQTT.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h> 

// MQTT client - using descriptive client ID and topic
// #define CLIENT_ID "esp32-sensors"
// #define TOPIC_PREFIX "706/ece140/sensors"

Adafruit_BMP085 bmp;

// WiFi credentials
const char* ucsdUsername = UCSD_USERNAME;
const char* ucsdPassword = UCSD_PASSWORD;
const char* wifiSsid = WIFI_SSID;
const char* nonEnterpriseWifiPassword = NON_ENTERPRISE_WIFI_PASSWORD;
const char* clientId = CLIENT_ID;
const char* topicPrefix = TOPIC_PREFIX;

ECE140_MQTT mqtt(clientId, topicPrefix);


void setup() {
    Serial.begin(115200);
    // ECE140_WIFI wifi;

    // // Connect to WiFi
    // wifi.connectToWiFi(wifiSsid, nonEnterpriseWifiPassword);

    // // Wait until WiFi is connected
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(1000);
    //     Serial.println("Connecting to WiFi...");
    // }
    // Serial.println("WiFi connected!");

    // // Connect to MQTT broker
    // if (mqtt.connectToBroker()) {
    //     mqtt.publishMessage("status", "Sensor device connected");
    //     Serial.println("Connected to MQTT Broker");
    // } else {
    //     Serial.println("Failed to connect to MQTT broker");
    // }
    
    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {}
    } else {
        Serial.println("BMP085 initialized successfully.");
    }

}

void loop() {


    float temp = bmp.readTemperature();
    float pressure = bmp.readPressure();

    String payload = "{ \"temperature\":" + String(temp) +
                        ", \"pressure\":" + String(pressure) + "}";
    
    if (mqtt.publishMessage("readings", payload)){
        Serial.println("Successfully published");
    } else{
        Serial.println("Unsuccessfully sent");
    }
    delay(5000);    
}