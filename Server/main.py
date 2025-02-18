import paho.mqtt.client as mqtt
import json
from datetime import datetime
from collections import deque
import numpy as np

# MQTT Broker settings
BROKER = "broker.hivemq.com"
PORT = 1883
BASE_TOPIC = "ENTER_SOMETHING_UNIQUE_HERE_THAT_SHOULD_ALSO_MATCH_MAINCPP/ece140/sensors"
TOPIC = BASE_TOPIC + "/#"

if BASE_TOPIC == "ENTER_SOMETHING_UNIQUE_HERE_THAT_SHOULD_ALSO_MATCH_MAINCPP/ece140/sensors":
    print("Please enter a unique topic for your server")
    exit()


def on_connect(client, userdata, flags, rc):
    """Callback for when the client connects to the broker."""
    if rc == 0:
        print("Successfully connected to MQTT broker")
        client.subscribe(TOPIC)
        print(f"Subscribed to {TOPIC}")
    else:
        print(f"Failed to connect with result code {rc}")

def on_message(client, userdata, msg):
    try:
        # Parse JSON message
        payload = json.loads(msg.payload.decode())
        current_time = datetime.now()

        # Check the topic if it matches the base topic + '/readings'
        if msg.topic == BASE_TOPIC + "/readings":
            print(f"Received data at {current_time}:")
            print(json.dumps(payload, indent=4))  # Pretty print the JSON payload
            
    except json.JSONDecodeError:
        print(f"\nReceived non-JSON message on {msg.topic}:")
        print(f"Payload: {msg.payload.decode()}")



def main():
    # Create MQTT client
    print("Creating MQTT client...")

    # Set the callback functions onConnect and onMessage
    print("Setting callback functions...")
    
    try:
        # Connect to broker
        print("Connecting to broker...")
        
        # Start the MQTT loop
        print("Starting MQTT loop...")
        
    except KeyboardInterrupt:
        print("\nDisconnecting from broker...")
        # make sure to stop the loop and disconnect from the broker
        print("Exited successfully")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()