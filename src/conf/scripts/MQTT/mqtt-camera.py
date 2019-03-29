#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import os
import logging

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

def on_connect(client, userdata, flags, rc):
    logger.info("MQTT connected with result code: " + str(rc))
    client.subscribe("myHome/camera")

def on_message(client, userdata, msg):
    command = msg.payload.decode("utf-8")
    if (command == "ON"):
        logger.info ("Camera ON")
        os.system("sudo systemctl start motion")
    if (command == "OFF"):
        os.system("sudo systemctl stop motion")
        logger.info ("Camera OFF")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("openhab.master", 1883, 60)

client.loop_forever()
