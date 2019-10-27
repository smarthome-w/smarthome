#!/bin/bash
sleep 20
echo Bedroom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:10:0C:C7 unlock'
echo AliceRoom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/aliceroomradin/trv' -m '00:1A:22:10:09:0C unlock'
echo FamilyRoom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/familyroomradin/trv' -m '00:1A:22:0C:85:FA unlock'
sleep 20
echo AnnRoom
/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/bedroomradin/trv' -m '00:1A:22:10:0C:7B unlock'
#echo Kitchen
#/usr/bin/mosquitto_pub -h 192.168.0.4 -t '/familyroomradin/trv' -m '00:1A:22:0E:06:A0 unlock'
echo done
