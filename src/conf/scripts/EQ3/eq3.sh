#!/bin/bash
#Bedroom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:10:0C:C7 unlock'
sleep 20
#AnnRoom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:10:0C:7B unlock'
sleep 20
#AliceRoom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:10:09:0C unlock'
sleep 20
#FamilyRoom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:0C:85:FA unlock'
sleep 20
#Kitchen
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:0E:06:A0 unlock'
