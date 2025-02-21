import paho.mqtt.client as mqtt
import json
from datetime import datetime
import os
import requests
import time
from dotenv import load_dotenv

# Load environment variables
dotenv_path = os.path.join(os.path.dirname(__file__), '.env')
load_dotenv(dotenv_path)

# MQTT Broker settings
BROKER = "broker.hivemq.com"
PORT = 1883
BASE_TOPIC = os.getenv("BASE_TOPIC", "default_topic/ece140/sensors")
if BASE_TOPIC == "default_topic/ece140/sensors":
    print("Warning: BASE_TOPIC is not set in the .env file. Using default value.")
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
        try:
            data = json.loads(payload)  # Safely parse JSON payload
        except json.JSONDecodeError:
            print("Error: Invalid JSON payload")
            return

        # Extract temperature value
        temperature = data.get("temperature")
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
            try:
                response = requests.post("http://localhost:5000/api/temperature", json=post_data)
                if response.status_code == 200:
                    print("POST request successful!")
                else:
                    print(f"POST request failed with status code {response.status_code}")
            except requests.exceptions.RequestException as e:
                print(f"Error sending POST request: {e}")

            # Update the last POST time
            last_post_time = current_time

    except Exception as e:
        print(f"Error processing message: {e}")

# MQTT Callback: When connected to the broker
def on_connect(client, userdata, flags, rc, properties=None):
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