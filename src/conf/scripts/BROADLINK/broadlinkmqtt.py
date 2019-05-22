#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import os
import logging
import time
import codecs
from ruamel.yaml import YAML
import broadlink

MESSAGE_PREFIX = "broadlink"
RMPRO_IP = None
RMPRO_MAC = None

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

mappings = None


def setup_network():
    global RMPRO_IP
    global RMPRO_MAC

    data_dir = os.path.dirname(os.path.abspath(__file__))
    data_dir = os.path.abspath(os.path.dirname(
        data_dir + "/../../broadlink-data/"))

    content = open(data_dir + "/system.yml", "r").read()
    yaml = YAML()
    system_dictionary = yaml.load(content)
    RMPRO_IP = system_dictionary["HW_RMPro"]
    RMPRO_MAC = system_dictionary["HW_RMPro_MAC"]


def setup():
    global mappings
    setup_network()
    logger.info("setup")

    if (mappings is None):
        mappings = {}

    data_dir = os.path.dirname(os.path.abspath(__file__))
    data_dir = os.path.abspath(os.path.dirname(
        data_dir + "/../../broadlink-data/"))

    content = open(data_dir + "/tc2.yml", "r").read()
    yaml = YAML()
    mappings["TC2"] = yaml.load(content)

    content = open(data_dir + "/irb.yml", "r").read()
    mappings["IRB"] = yaml.load(content)

    content = open(data_dir + "/ir_colors.yml", "r").read()
    mappings["IRCOLORS"] = yaml.load(content)


def send_broadlink_code(code, retry_num=1):
    global RMPRO_IP
    global RMPRO_MAC
    if (code != ""):
        try:
            # settings
            device = broadlink.rm(
                host=(RMPRO_IP, 80), mac=bytearray.fromhex(RMPRO_MAC), devtype="rm")
            device.auth()
            for i in range(retry_num):
                decode_hex = codecs.getdecoder("hex_codec")
                device.send_data(decode_hex(code)[0])
            return True
        except Exception as e:
            logger.error(e)
            return False
    else:
        logger.error("Empty code value")


def on_connect(client, userdata, flags, rc):
    logger.info("MQTT connected with result code: " + str(rc))
    client.subscribe(MESSAGE_PREFIX + "/#")


def on_message(client, userdata, msg):
    global mappings
    #    global CURRENT_POSITION
    tokenized_topic = str(msg.topic).split("/")
    type = tokenized_topic[1]
    element = tokenized_topic[2]
    command = msg.payload.decode("utf-8")
    code = None

    ret_num = 1
    if (type == "IRCOLORS"):
        ret_num = 5

    try:
        code = mappings[type]["mapping_dictionary"][element][command]
        setup_network()
        if (not send_broadlink_code(code, ret_num)):
            logger.error("Message type:{} element:{} command:{} status: NOT SENT".format(
                type, element, command))
        else:
            logger.info("Message type:{} element:{} command:{} status: SENT".format(
                type, element, command))
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
