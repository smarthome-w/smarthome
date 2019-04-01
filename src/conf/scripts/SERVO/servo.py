#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import os
import logging
import time
import RPi.GPIO as GPIO

CURRENT_POSITION = 50

PWM_PIN = 21

PWM_MIN = 2.5
PWM_MAX = 12.5
PWM_NEUTRAL = 7.5

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

try:
    f = open("/tmp/servo", "r")
    CURRENT_POSITION = int(f.read())
except:
    CURRENT_POSITION = 50

logger.info("Initialization: set current position to {}".format(CURRENT_POSITION))

def on_connect(client, userdata, flags, rc):
    logger.info("MQTT connected with result code: " + str(rc))
    client.subscribe("myHome/GF_FamilyRoom_Camera_Servo")

def calculate_pwm(position):
    return PWM_MIN + float(position) * (PWM_MAX - PWM_MIN) / 100

def on_message(client, userdata, msg):
    global CURRENT_POSITION
    command = msg.payload.decode("utf-8")
    desired_position = int(command)

    if (CURRENT_POSITION != desired_position):
        logger.info("Current value: {} does not equal desired position: {}".format(CURRENT_POSITION, desired_position))

        GPIO.setmode(GPIO.BCM)
        GPIO.setup(PWM_PIN, GPIO.OUT)
        p = GPIO.PWM(PWM_PIN, 50)
        logger.info("Initialize servo at current position to {}".format(CURRENT_POSITION))
        p.start (calculate_pwm(CURRENT_POSITION))

        change = desired_position - CURRENT_POSITION

        step = 2
        if (change < 0):
            step = -2

        logger.info("Step value: {}, change: {}".format(step, change))

        while abs(desired_position - CURRENT_POSITION) > 0:
            CURRENT_POSITION = CURRENT_POSITION + step
            if (step > 0 and CURRENT_POSITION > desired_position) or (step < 0 and CURRENT_POSITION < desired_position):
                CURRENT_POSITION = desired_position
            logger.info("Change current position to {}, {}".format(CURRENT_POSITION, calculate_pwm(CURRENT_POSITION)))
            p.ChangeDutyCycle(calculate_pwm(CURRENT_POSITION));
            time.sleep(0.1)
        logger.info("Final change current position to {}, {}".format(CURRENT_POSITION, calculate_pwm(CURRENT_POSITION)))
        p.ChangeDutyCycle(calculate_pwm(CURRENT_POSITION));
        p.stop();
        GPIO.cleanup()
        logger.info("Servo is set to: {}, {}".format(CURRENT_POSITION, calculate_pwm(CURRENT_POSITION)))
        f = open("/tmp/servo", "w")
        f.write(str(CURRENT_POSITION))
    else:
        logger.info("Current value: {} equals desired position: {}".format(CURRENT_POSITION, desired_position))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("openhab.master", 1883, 60)

client.loop_forever()
