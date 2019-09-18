#!/bin/bash
#Bedroom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:10:0C:C7 unlock'
sleep 20
#AnnRoom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:10:0C:7B unlock'
