#include "ECE140_WIFI.h"
#include "ECE140_MQTT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define CLIENT_ID "esp32-sensors"
#define TOPIC_PREFIX "ece140/sensors"
ECE140_MQTT mqtt(CLIENT_ID, TOPIC_PREFIX);

const char* ucsdUsername = UCSD_USERNAME;
const char* ucsdPassword = UCSD_PASSWORD;
const char* wifiSsid = WIFI_SSID;
const char* nonEnterpriseWifiPassword = NON_ENTERPRISE_WIFI_PASSWORD;

Adafruit_BMP085 bmp;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }

    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {
            delay(10);
        }
    }

    Serial.println("BMP085 Sensor Initialized Successfully");
}

void loop() {
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0;

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");

    delay(1000);
}