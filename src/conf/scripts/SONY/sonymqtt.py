#!/usr/bin/env python3
from braviarc.braviarc import BraviaRC
from ruamel.yaml import YAML
import paho.mqtt.client as mqtt
import logging
import time
import codecs
import sys
import os

MESSAGE_PREFIX = "sony"
SONY_IP = None
SONY_PIN = '9209'
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


def setup_network():
    global SONY_IP

    data_dir = os.path.dirname(os.path.abspath(__file__))
    data_dir = os.path.abspath(os.path.dirname(
        data_dir + "/../../broadlink-data/"))

    content = open(data_dir + "/sony.yml", "r").read()
    yaml = YAML()
    system_dictionary = yaml.load(content)
    SONY_IP = system_dictionary["sony_ip"]


def setup():
    global SONY_IP
    setup_network()
    logger.info("setup")

    braviarc = BraviaRC(SONY_IP)
    pin = SONY_PIN
    braviarc.connect(pin, 'sony', 'sony')


def on_connect(client, userdata, flags, rc):
    logger.info("MQTT connected with result code: " + str(rc))
    client.subscribe(MESSAGE_PREFIX + "/#")


def on_message(client, userdata, msg):
    global SONY_IP
    global SONY_PIN
    #    global CURRENT_POSITION
    tokenized_topic = str(msg.topic).split("/")
    type = tokenized_topic[1]
    element = tokenized_topic[2]
    command = msg.payload.decode("utf-8")

    logger.info("Message type:{} element:{} command:{}".format(
        type, element, command))

    try:
        braviarc = BraviaRC(SONY_IP)
        pin = SONY_PIN
        braviarc.connect(pin, 'sony', 'sony')
        if (braviarc.is_connected()):
            if (element == "RTV_TV_Input"):
                playing_content = braviarc.get_playing_info()
                if (command.startswith("HDMI")):
                    source = command.replace("__", " ")
                    source = source.replace("_", "/")
                    braviarc.select_source(source)
                    logger.info("sent: {}".format(source))
                elif (command.startswith("NETFLIX")):
                    braviarc.start_app("Netflix")
                elif (command.startswith("YOUTUBE")):
                    braviarc.start_app("YouTube")
        else:
            logger.error("TV not connected")
    except Exception as e:
        logger.info("ERR Message type:{} element:{} command:{}".format(
            type, element, command))
        logger.error(e)


setup()
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("openhab.master", 1883, 60)

client.loop_forever()
