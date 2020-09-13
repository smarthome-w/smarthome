#!/usr/bin/env python3

import json
import paho.mqtt.client as mqtt
import os
import logging
from ruamel.yaml import YAML
import requests
import xmltodict
import pprint
import datetime
from datetime import timedelta
import json
import time

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

ZULU_DATE_FORMAT_STRING = "%Y-%m-%dT%H:%M:%SZ"
DAY_FORMAT_STRING = "%Y-%m-%d"
MIN_TEMP_NOT_DEFINED = 273.0
MAX_TEMP_NOT_DEFINED = -273.0

MQTT_TOPIC_NAME = "weather/info"


class DailyForecast:
    def __init__(self):
        self.day = None
        self.min_temp = MIN_TEMP_NOT_DEFINED
        self.max_temp = MAX_TEMP_NOT_DEFINED
        self.precipitation_total = 0.0
        self.precipitation_min_total = 0.0
        self.precipitation_max_total = 0.0
        self.precipitation_kind = None

    def __repr__(self):
        return "day: {}, precipitation_min_total: {}, precipitation_max_total: {}, precipitation_total: {}, min_temp: {}, max_temp: {}".format(
            self.day, self.precipitation_min_total, self.precipitation_max_total, self.precipitation_total, self.min_temp, self.max_temp)


class Forecast:
    def __init__(self):
        base_path = os.path.dirname(os.path.abspath(__file__))

        self._detailed_forecasts = {}
        self._normal_forecasts = {}

        with open(os.path.join(base_path, "data_link.sensitive"), "r") as text_file:
            yaml = YAML()
            self._config_data = yaml.load(text_file)
            url = self._config_data["link"]

        logger.info("Forecast at: {}".format(url))

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
                        element = DailyForecast()
                        element.day = a_date
                        self._detailed_forecasts[a_date] = element

                    precipitation = weather_value["location"]["precipitation"]
                    self._detailed_forecasts[a_date].precipitation_total += float(
                        precipitation["@value"])
                    self._detailed_forecasts[a_date].precipitation_min_total += float(
                        precipitation["@minvalue"])
                    self._detailed_forecasts[a_date].precipitation_max_total += float(
                        precipitation["@maxvalue"])
                else:
                    # normal forecast
                    if a_date not in self._normal_forecasts:
                        element = DailyForecast()
                        element.day = a_date
                        self._normal_forecasts[a_date] = element
                    precipitation = weather_value["location"]["precipitation"]
                    self._normal_forecasts[a_date].precipitation_total += float(
                        precipitation["@value"])
                    self._normal_forecasts[a_date].precipitation_min_total += float(
                        precipitation["@minvalue"])
                    self._normal_forecasts[a_date].precipitation_max_total += float(
                        precipitation["@maxvalue"])
                    min_temp = weather_value["location"]["minTemperature"]
                    max_temp = weather_value["location"]["maxTemperature"]
                    if float(min_temp["@value"]) < self._normal_forecasts[a_date].min_temp:
                        self._normal_forecasts[a_date].min_temp = float(
                            min_temp["@value"])
                    if float(max_temp["@value"]) > self._normal_forecasts[a_date].max_temp:
                        self._normal_forecasts[a_date].max_temp = float(
                            max_temp["@value"])

        logger.info("detailed forecasts: {}".format(self._detailed_forecasts))
        logger.info("normal forecasts: {}".format(self._normal_forecasts))

    def _get_today(self):
        return datetime.date.today()

    def _get_tomorrow(self):
        return datetime.date.today() + datetime.timedelta(days=1)

    def calculate_message(self):
        message_map = {}
        curr_date_str = self._get_today().strftime(DAY_FORMAT_STRING)
        tomorrow_date_str = self._get_tomorrow().strftime(DAY_FORMAT_STRING)
        logger.info("curr_date: {}".format(curr_date_str))
        logger.info("tomorrow_date: {}".format(tomorrow_date_str))
        return message_map

    def send_message(self, messages):
        logger.info("Send messages: {}".format(messages))

        client = mqtt.Client()
        client.connect("openhab.master", 1883, 60)
        client.publish("{}".format(MQTT_TOPIC_NAME),
                       "{}".format(messages))


runner = Forecast()
message = runner.calculate_message()
runner.send_message(message)
