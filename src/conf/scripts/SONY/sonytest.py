#!/usr/bin/env python3
import codecs
import logging
import os
import sys
import time

import paho.mqtt.client as mqtt
from ruamel.yaml import YAML

from braviarc.braviarc import BraviaRC

MESSAGE_PREFIX = "sony"
SONY_IP = None
SONY_PIN = '1779'

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
    braviarc.connect(pin, 'sony', 'sonyrc')


setup()

braviarc = BraviaRC(SONY_IP)
pin = SONY_PIN
braviarc.connect(pin, 'sony', 'sonyrc')
if (braviarc.is_connected()):
#    volume_info = braviarc.get_volume_info()
#    print(volume_info)
#    print('==')
#    print(volume_info.get('volume'))
#    print('==')
#    braviarc.set_volume_level(0.29)
#    # braviarc.volume_up()
#    print('==')
#    system_info = braviarc.get_system_info()
#    print(system_info)
#    print('==')
    system_info = braviarc.load_source_list()
    print(system_info)
