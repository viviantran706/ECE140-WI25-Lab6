#include "ECE140_WIFI.h"
#include "ECE140_MQTT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>



// #define CLIENT_ID "esp32-sensors"
// #define TOPIC_PREFIX "ece140/sensors"


const char* ucsdUsername = UCSD_USERNAME;
const char* ucsdPassword = UCSD_PASSWORD;
const char* wifiSsid = WIFI_SSID;
const char* nonEnterpriseWifiPassword = NON_ENTERPRISE_WIFI_PASSWORD;
const char* clientId = CLIENT_ID;
const char* topicPrefix = TOPIC_PREFIX;

ECE140_MQTT mqtt(clientId, topicPrefix);

// Initialize BMP sensor
Adafruit_BMP085 bmp;

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    while (!Serial) {
        delay(10); // Wait for serial port to connect
    }

    // Initialize BMP sensor
    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {
            delay(10); // Halt execution if sensor initialization fails
        }
    }

    Serial.println("BMP085 Sensor Initialized Successfully");
}

void loop() {
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure();

    // Create a JSON-like payload
    String payload = "{\"temperature\": " + String(temperature) +
                     ", \"pressure\": " + String(pressure) + "}";

    // Publish the payload to the MQTT broker
    if (mqtt.publishMessage("readings", payload.c_str())) {
        Serial.println("Message sent successfully!");
    } else {
        Serial.println("Failed to send message!");
    }

    delay(5000); // Send data every 5 seconds
}