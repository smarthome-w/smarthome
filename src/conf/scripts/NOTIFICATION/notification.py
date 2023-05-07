#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import os
import logging
import time
import urllib
import http
from ruamel.yaml import YAML


PUSHOVER = None

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

def setup():
    global PUSHOVER
    data_dir = os.path.dirname(os.path.abspath(__file__))
    content = open(data_dir + "/notification.yml.sensitive", "r").read()
    yaml = YAML()
    system_dictionary = yaml.load(content)
    PUSHOVER = system_dictionary['pushover']

def on_connect(client, userdata, flags, rc):
    logger.info("MQTT connected with result code: " + str(rc))
    client.subscribe("notification/string")


def on_message(client, userdata, msg):
    command = msg.payload.decode("utf-8")
    logger.info("command: {}".format(command))

    message = command.split('|')
    msg_type = message[0]
    msg_item_name = message[1]
    msg_item_value = message[2]
    msg_current_time = message[3]
    msg_enable_notification = message[4]

    msg_priority = -2
    if (msg_enable_notification == 'ON'):
        msg_priority = 1
    msg_title = "title"
    msg_content = command

    if (msg_type == 'presence'):
        msg_content = msg_current_time
        person_name = msg_item_name.split("_")[0]
        if ('AtHome' in msg_item_name):
            if (msg_item_value == 'OPEN'):
                msg_title = "{} w domu".format(person_name)
            else:
                msg_title = "{} poza domem".format(person_name)

        if ('AtSchool' in msg_item_name):
            if (msg_item_value == 'OPEN'):
                msg_title = "{} w szkole".format(person_name)
            else:
                msg_title = "{} poza szkołą".format(person_name)

        if ('AtDistrict' in msg_item_name):
            if (msg_item_value == 'OPEN'):
                msg_title = "{} w dzielnicy".format(person_name)
            else:
                msg_title = "{} poza dzielnicą".format(person_name)

        if ('InTransit' in msg_item_name):
            if (msg_item_value == 'OPEN'):
                msg_title = "{} na pętli".format(person_name)
            else:
                msg_title = "{} poza pętlą".format(person_name)

    if (msg_type == 'garage'):
        msg_content = msg_current_time
        msg_priority = 1
        if (msg_item_value == 'OPEN'):
            msg_title = "Garaż otwarty"
        else:
            msg_title = "Garaż zamknięty"

    if (msg_type == 'error'):
        msg_content = "{} {}".format(msg_item_name, msg_current_time)
        if (msg_item_value == 'ON'):
            msg_title = "Błąd"
        else:
            msg_title = "OK"

        if ('OverallErrorIndicator' in msg_item_name):
            if (msg_item_value == 'ON'):
                msg_title = "Błąd czujników"
            else:
                msg_title = "Czujniki sprawne"
            msg_content = "{}".format(msg_current_time)

    if (msg_type == 'alarm'):
        msg_content = "{} {}".format(msg_item_name, msg_current_time)
        if (msg_item_value == 'OPEN'):
            msg_title = "Otwarte"
        else:
            msg_title = "Zamknięte"

    if (msg_type == 'information'):
        msg_content = "{} {}".format(msg_item_name, msg_current_time)
        msg_title = msg_item_value
        if (msg_item_value == 'OPEN'):
            msg_title = "Otwarte"
        else:
            msg_title = "Zamknięte"

        if ('MainLock' in msg_item_name):
            msg_content = msg_current_time
            if (msg_item_value == 'OPEN'):
                msg_title = "Zamek otwarty"
            else:
                msg_title = "Zamek zamknięty"

        if ('MainDoor' in msg_item_name):
            msg_content = msg_current_time
            if (msg_item_value == 'OPEN'):
                msg_title = "Drzwi otwarte"
            else:
                msg_title = "Drzwi zamknięte"

    logger.info("message: {}, msg_content: {}, msg_title: {}, msg_piority: {}".format(message, msg_content, msg_title, msg_priority))

    conn = http.client.HTTPSConnection("api.pushover.net:443")
    conn.request("POST", "/1/messages.json",
    urllib.parse.urlencode({
        "token": PUSHOVER['pushover_application'],
        "user": PUSHOVER['pushover_user_key'],
        "message": msg_content,
        "title": msg_title,
        "priority": msg_priority
    }), { "Content-type": "application/x-www-form-urlencoded" })

    response = conn.getresponse()
    logger.info("response: {}".format(response.getcode()))

setup()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("openhab.master", 1883, 60)

client.loop_forever()
