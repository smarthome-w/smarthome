#!/usr/bin/python3

import logging
import os
from datetime import datetime
from datetime import timezone

from ruamel.yaml import YAML
import paho.mqtt.client as mqtt
from locationsharinglib import Service
from math import sin, cos, sqrt, atan2, radians

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

ZULU_DATE_FORMAT_STRING = "%Y-%m-%dT%H:%M:%SZ"
MESSAGE_PREFIX = "location"

COOKIES_FILE = None
GOOGLE_EMAIL = None
PRESENCE_ZONES = None

MQTT_CLIENT = None


def calculate_distance(lat1, lon1, lat2, lon2):
    R = 6373.0

    lat1 = radians(lat1)
    lon1 = radians(lon1)
    lat2 = radians(lat2)
    lon2 = radians(lon2)

    dlon = lon2 - lon1
    dlat = lat2 - lat1

    a = sin(dlat / 2)**2 + cos(lat1) * cos(lat2) * sin(dlon / 2)**2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))

    distance = R * c
    return distance


def utc_to_local(utc_dt):
    return utc_dt.replace(tzinfo=timezone.utc).astimezone(tz=None)


def setup():
    global MQTT_CLIENT
    global COOKIES_FILE
    global GOOGLE_EMAIL
    global PRESENCE_ZONES

    data_dir = os.path.dirname(os.path.abspath(__file__))
    content = open(data_dir + "/settings.yml.sensitive", "r").read()
    yaml = YAML()
    system_dictionary = yaml.load(content)

    GOOGLE_EMAIL = system_dictionary["google_email"]
    PRESENCE_ZONES = system_dictionary["presence_zones"]

    COOKIES_FILE = data_dir + "/google-cookie.txt.sensitive"
    MQTT_CLIENT = mqtt.Client()
    MQTT_CLIENT.connect("192.168.0.4", 1883, 60)

    logger.info("Cookies file: {}".format(COOKIES_FILE))
    logger.info("Google email: {}".format(GOOGLE_EMAIL))
    logger.info("Presence zones: {}".format(PRESENCE_ZONES))


def send_person_location(client, person):
    nickname = person.nickname
    if nickname.startswith("walde"):
        nickname = "Waldek"

    prefix = "{}/{}".format(MESSAGE_PREFIX, nickname)
    date_t = utc_to_local(person.datetime)
    date_t = str(date_t)
    date_t = date_t[0:19]

    battery_level = person.battery_level
    if not battery_level:
        battery_level = 0

    payload = "{" + "location:\"{},{}\",accuracy:{},datetime:\"{}\",battery_level:\"{}\",address:\"{}\"".format(
        person.latitude, person.longitude, person.accuracy, date_t, battery_level, person.address) + "}"

#    logger.info("prefix: {}, payload: {}".format(prefix, payload))
    send_message(prefix, payload)


def send_person_zones(client, person):
    nickname = person.nickname
    if nickname.startswith("walde"):
        nickname = "Waldek"

    prefix = "{}/{}".format(MESSAGE_PREFIX, nickname)
    presence = {}
    for zone in PRESENCE_ZONES:
        lat1 = person.latitude
        lon1 = person.longitude
        lat2 = PRESENCE_ZONES[zone]["latitude"]
        lon2 = PRESENCE_ZONES[zone]["longitude"]
        range = PRESENCE_ZONES[zone]["range"]
        distance = calculate_distance(lat1, lon1, lat2, lon2)
        if (distance <= range):
            presence[zone] = "OPEN"
        else:
            presence[zone] = "CLOSED"

    # logger.info("Nickname: {}, Zone: {}, distance: {}, presence: {}".format(
    #    nickname, zone, distance, presence[zone]))
#    logger.info("Presence for {}: {}".format(nickname, presence))
    payload = "{presence:" + str(presence) + "}"
    send_message(prefix, payload)


def send_heartbeat(client):
    now = datetime.now()
    time = now.strftime(ZULU_DATE_FORMAT_STRING)
    prefix = "{}/{}".format(MESSAGE_PREFIX, "heartbeat")
    payload = "{time:" + str(time) + "}"
    send_message(prefix, payload)


def send_message(prefix, payload):
    logger.info("mqtt {}: {}".format(prefix, payload))
    MQTT_CLIENT.publish(prefix, payload)


setup()
service = Service(cookies_file=COOKIES_FILE,
                  authenticating_account=GOOGLE_EMAIL)

send_heartbeat(MQTT_CLIENT)

for person in service.get_all_people():
    send_person_location(MQTT_CLIENT, person)
    send_person_zones(MQTT_CLIENT, person)


# /usr/bin/python3 /opt/openhab/conf/scripts/GOOGLELOCATION/location.py
