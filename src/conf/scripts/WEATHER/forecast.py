#!/usr/bin/env python3

import datetime
import json
import logging
import os
import pprint
import time
from datetime import timedelta

import paho.mqtt.client as mqtt
import requests
import xmltodict
from ruamel.yaml import YAML

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

ZULU_DATE_FORMAT_STRING = "%Y-%m-%dT%H:%M:%SZ"
DAY_FORMAT_STRING = "%Y-%m-%d"
MIN_TEMP_NOT_DEFINED = 273.0
MAX_TEMP_NOT_DEFINED = -273.0

MQTT_TOPIC_NAME = "weather/info"


class Forecast:
    def __init__(self):
        base_path = os.path.dirname(os.path.abspath(__file__))

        self._detailed_forecasts = {}
        self._normal_forecasts = {}

        with open(os.path.join(base_path, "data_link.sensitive"), "r") as text_file:
            yaml = YAML()
            self._config_data = yaml.load(text_file)
            url = self._config_data["link"]

        r = requests.get(url, allow_redirects=True)
        self._weather_dict = xmltodict.parse(
            r.content, process_namespaces=True)

        weather_data = self._weather_dict["weatherdata"]["product"]["time"]

        for weather_value in weather_data:
            if ("precipitation" in weather_value["location"]):
                from_date = datetime.datetime.strptime(
                    weather_value["@from"], ZULU_DATE_FORMAT_STRING)
                to_date = datetime.datetime.strptime(
                    weather_value["@to"], ZULU_DATE_FORMAT_STRING)
                a_date = from_date.strftime(DAY_FORMAT_STRING)
                forecast_duration = to_date - from_date
                if forecast_duration <= timedelta(hours=1):
                    if a_date not in self._detailed_forecasts:
                        element = {}
                        self._initialize_forecast_map(element)
                        element["day"] = a_date
                        self._detailed_forecasts[a_date] = element

                    precipitation = weather_value["location"]["precipitation"]
                    self._detailed_forecasts[a_date]["precip_total"] += float(
                        precipitation["@value"])
                    self._detailed_forecasts[a_date]["precip_min_total"] += float(
                        precipitation["@minvalue"])
                    self._detailed_forecasts[a_date]["precip_max_total"] += float(
                        precipitation["@maxvalue"])
                else:
                    # normal forecast
                    if a_date not in self._normal_forecasts:
                        element = {}
                        element["day"] = a_date
                        self._initialize_forecast_map(element)
                        self._normal_forecasts[a_date] = element

                    precipitation = weather_value["location"]["precipitation"]
                    self._normal_forecasts[a_date]["precip_total"] += float(
                        precipitation["@value"])
                    self._normal_forecasts[a_date]["precip_min_total"] += float(
                        precipitation["@minvalue"])
                    self._normal_forecasts[a_date]["precip_max_total"] += float(
                        precipitation["@maxvalue"])
                    min_temp = weather_value["location"]["minTemperature"]
                    max_temp = weather_value["location"]["maxTemperature"]
                    if float(min_temp["@value"]) < self._normal_forecasts[a_date]["min_temp"]:
                        self._normal_forecasts[a_date]["min_temp"] = float(
                            min_temp["@value"])
                    if float(max_temp["@value"]) > self._normal_forecasts[a_date]["max_temp"]:
                        self._normal_forecasts[a_date]["max_temp"] = float(
                            max_temp["@value"])

    def _initialize_forecast_map(self, a_map):
        a_map["precip_total"] = 0.0
        a_map["precip_min_total"] = 0.0
        a_map["precip_max_total"] = 0.0
        a_map["max_temp"] = MAX_TEMP_NOT_DEFINED
        a_map["min_temp"] = MIN_TEMP_NOT_DEFINED

    def _get_today(self):
        return datetime.date.today()

    def _get_tomorrow(self):
        return datetime.date.today() + datetime.timedelta(days=1)

    def calculate_messages(self):
        message_map = {}
        today_date_str = self._get_today().strftime(DAY_FORMAT_STRING)
        tomorrow_date_str = self._get_tomorrow().strftime(DAY_FORMAT_STRING)

        today_forecast = self._normal_forecasts[today_date_str]
        tomorrow_forecast = self._normal_forecasts[tomorrow_date_str]

        #logger.info("today_forecast: {}".format(today_forecast))
        message_map["today"] = today_forecast
        #logger.info("tomorrow_forecast: {}".format(tomorrow_forecast))
        message_map["tomorrow"] = tomorrow_forecast

        next_precip_date_str = ""
        next_precip_volume_str = ""
        next_possible_precip_date_str = ""
        next_possible_precip_volume_str = ""

        for key, value in self._normal_forecasts.items():
            if value["precip_total"] > 0.0 and next_precip_date_str == "":
                next_precip_date_str = key
                next_precip_volume_str = "{:4.1f} mm".format(
                    value["precip_total"])

            if value["precip_max_total"] > 0.0 and next_possible_precip_date_str == "":
                next_possible_precip_date_str = key
                next_possible_precip_volume_str = "{:4.1f} mm".format(
                    value["precip_max_total"])

        message_map["next_precip_date"] = next_precip_date_str
        message_map["next_precip_volume"] = next_precip_volume_str
        message_map["next_possible_precip_date"] = next_possible_precip_date_str
        message_map["next_possible_precip_volume"] = next_possible_precip_volume_str
        return message_map

    def send_messages(self, messages):
        logger.info("Send messages: {}".format(messages))

        messages_str = "{}".format(messages).replace('\'', '"')
        client = mqtt.Client()
        client.connect("openhab.master", 1883, 60)
        client.publish("{}".format(MQTT_TOPIC_NAME),
                       "{}".format(messages_str))


runner = Forecast()
messages = runner.calculate_messages()
runner.send_messages(messages)
