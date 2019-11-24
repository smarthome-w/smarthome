#!/bin/bash -e

while [ "true" = "true" ]; do
    temperature=$(cat /sys/class/thermal/thermal_zone0/temp)
    idle=$(/usr/bin/top -b -n 1 | /bin/sed -n "s/^%Cpu.*ni, \([0-9.]*\) .*$/\1/p")
    uptime=$(uptime --pretty)
    DATE=$(date '+%Y-%m-%d %H:%M:%S')

    temperature=$((temperature / 1000))

    /usr/bin/mosquitto_pub -t myHome/serverTemperature -m "${temperature}" -h "openhab.master"
    /usr/bin/mosquitto_pub -t myHome/serverIdle -m "${idle}" -h "openhab.master"
    /usr/bin/mosquitto_pub -t myHome/serverUptime -m "${uptime}" -h "openhab.master"
    /usr/bin/mosquitto_pub -t myHome/lastReadServer -m "${DATE}" -h "openhab.master"

    echo ">>${temperature}:${idle}:${uptime}:${DATE}"
    sleep 120
done
