import paho.mqtt.client as mqtt
from datetime import datetime
import os
import requests
import time
from dotenv import load_dotenv

# Load environment variables
load_dotenv()

# MQTT Broker settings
BROKER = "broker.hivemq.com"
PORT = 1883
BASE_TOPIC = os.getenv("BASE_TOPIC", "your_unique_topic/ece140/sensors")
if BASE_TOPIC == "your_unique_topic/ece140/sensors":
    print("Please enter a unique topic for your server in the .env file")
    exit()

# Full topic for subscription
TOPIC = f"{BASE_TOPIC}/readings"

# Track the last time a POST request was sent
last_post_time = 0
post_interval = 5  # Minimum interval between POST requests (in seconds)

# MQTT Callback: When a message is received
def on_message(client, userdata, msg):
    global last_post_time
    try:
        # Decode the message payload
        payload = msg.payload.decode()
        print(f"Received message: {payload}")

        # Parse the JSON-like payload
        data = eval(payload)  # Use json.loads(payload) if the payload is valid JSON
        temperature = data.get("temperature")  # Ensure this matches the key sent by the ESP32
        if temperature is None:
            print("Error: 'temperature' key not found in payload")
            return

        # Generate timestamp
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # Check if enough time has passed since the last POST request
        current_time = time.time()
        if current_time - last_post_time >= post_interval:
            # Prepare the POST request body
            post_data = {
                "value": temperature,
                "unit": "C",
                "timestamp": timestamp
            }

            # Send the POST request to the web server
            response = requests.post("http://localhost:5000/api/temperature", json=post_data)

            # Check the response status
            if response.status_code == 200:
                print("POST request successful!")
            else:
                print(f"POST request failed with status code {response.status_code}")

            # Update the last POST time
            last_post_time = current_time

    except Exception as e:
        print(f"Error processing message: {e}")

# MQTT Callback: When connected to the broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker successfully!")
        # Subscribe to the topic
        client.subscribe(TOPIC)
        print(f"Subscribed to topic: {TOPIC}")
    else:
        print(f"Failed to connect to MQTT broker with result code {rc}")

def main():
    # Create MQTT client
    print("Creating MQTT client...")
    client = mqtt.Client()

    # Set the callback functions
    client.on_connect = on_connect
    client.on_message = on_message

    try:
        # Connect to broker
        print("Connecting to broker...")
        client.connect(BROKER, PORT, 60)

        # Start the MQTT loop
        print("Starting MQTT loop...")
        client.loop_forever()

    except KeyboardInterrupt:
        print("\nDisconnecting from broker...")
        client.disconnect()
        print("Exited successfully")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()