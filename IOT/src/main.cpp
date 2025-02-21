#include "ECE140_WIFI.h"
#include "ECE140_MQTT.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h> 

// MQTT client - using descriptive client ID and topic
#define CLIENT_ID "esp32-sensors"
#define TOPIC_PREFIX "ECE140a/ece140/sensors"

Adafruit_BMP085 bmp;

ECE140_MQTT mqtt(CLIENT_ID, TOPIC_PREFIX);

// WiFi credentials
const char* ucsdUsername = UCSD_USERNAME;
const char* ucsdPassword = UCSD_PASSWORD;
const char* wifiSsid = WIFI_SSID;
const char* nonEnterpriseWifiPassword = NON_ENTERPRISE_WIFI_PASSWORD;

void setup() {
    Serial.begin(115200);
    ECE140_WIFI wifi;

    // Connect to WiFi
    wifi.connectToWiFi(wifiSsid, nonEnterpriseWifiPassword);

    // Wait until WiFi is connected
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");

    // Connect to MQTT broker
    if (mqtt.connectToBroker()) {
        mqtt.publishMessage("status", "Sensor device connected");
        Serial.println("Connected to MQTT Broker");
    } else {
        Serial.println("Failed to connect to MQTT broker");
    }
    int32_t readPressure(void);

    //Making sure serial port cocnnect
    Serial.begin(9600);
    if (!bmp.begin()) {
	    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	    while (1) {}
    }
}

void loop() {
    // mqtt.loop();  // Keep the MQTT connection alive

    // // Read the sensors
    // int hallValue = hallRead();  // Ensure hallRead() is defined elsewhere
    // float temperature = temperatureRead();  // Ensure temperatureRead() is defined elsewhere

    // // Debugging: Print sensor values to Serial Monitor
    // Serial.print("Hall Sensor Value: ");
    // Serial.println(hallValue);
    // Serial.print("Temperature: ");
    // Serial.println(temperature);

    // // Create a JSON message
    // String message = "{\"hall_sensor\": " + String(hallValue) + 
    //                  ", \"temperature\": " + String(temperature) + "}";

    // // Send the message via MQTT
    // if (mqtt.publishMessage("sensor/data", message.c_str())) {
    //     Serial.println("Message sent successfully!");
    // } else {
    //     Serial.println("Failed to send message!");
    // }

    // Delay to prevent flooding the MQTT server with messages
    //delay(5000);  // Send a message every 5 seconds (adjust as needed)

    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
    
    Serial.println();
    delay(500);
}