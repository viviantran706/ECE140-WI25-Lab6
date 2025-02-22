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
    // Read temperature and pressure from the sensor
    float temperature = bmp.readTemperature(); // Temperature in °C
    float pressure = bmp.readPressure() / 100.0; // Pressure in hPa

    // Format payload as JSON-like string
    String payload = "{\"temperature\": " + String(temperature) + ", \"pressure\": " + String(pressure) + "}";

    // Publish payload to MQTT topic
    mqtt.publishMessage("readings", payload);

    // Print payload to Serial Monitor for debugging
    Serial.print("Published Payload: ");
    Serial.println(payload);

    // Wait for a second before taking the next reading
    delay(1000);
}