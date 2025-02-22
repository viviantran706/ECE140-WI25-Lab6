import paho.mqtt.client as mqtt
import requests
import os
import time
from dotenv import load_dotenv

load_dotenv()

BASE_TOPIC = os.getenv('BASE_TOPIC')

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(BASE_TOPIC + "/readings")

def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(f"Received `{payload}` from `{msg.topic}` topic")
    
    # Parse JSON payload
    data = eval(payload)
    temp_value = data['temperature']
    
    # Send POST request every 5 seconds
    current_time = time.time()
    if current_time - on_message.last_request_time >= 5:
        response = requests.post('http://localhost:6543/temperature', json={
            'value': temp_value,
            'unit': 'C',
            'timestamp': time.strftime('%Y-%m-%d %H:%M:%S', time.gmtime())
        })
        print(response.text)
        on_message.last_request_time = current_time

on_message.last_request_time = 0

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("broker.hivemq.com", 1883, 60)

client.loop_forever()