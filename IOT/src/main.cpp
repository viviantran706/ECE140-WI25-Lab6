#include "ECE140_WIFI.h"
#include "ECE140_MQTT.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h> 

// MQTT client - using descriptive client ID and topic
// #define CLIENT_ID "esp32-sensors"
#define TOPIC_PREFIX "706/ece140/sensors"
WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_BMP085 bmp;

// WiFi credentials
const char* ucsdUsername = UCSD_USERNAME;
const char* ucsdPassword = UCSD_PASSWORD;
const char* wifiSsid = WIFI_SSID;
const char* nonEnterpriseWifiPassword = NON_ENTERPRISE_WIFI_PASSWORD;
const char* clientId = CLIENT_ID;
const char* topicPrefix = TOPIC_PREFIX;

ECE140_MQTT mqtt(clientId, topicPrefix);

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  
  void reconnect() {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      if (client.connect("ESP32Client")) {
        Serial.println("connected");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
void setup() {
    
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
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
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