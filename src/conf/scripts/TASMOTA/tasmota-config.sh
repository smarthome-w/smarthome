#!/bin/bash

function uriencode() {
    jq -nr --arg v "$1" '$v|@uri'
}

echo -n "Finding Espressif devices on local network... "
IP_ADDRS=$(arp-scan -I eth0 192.168.0.0/24 -x | awk '{ print $1 }')
echo $(echo "$IP_ADDRS" | wc -l)" devices found"

for IP in $IP_ADDRS; do
    echo "***********************"
    echo "Working on $IP"
    echo "***********************"

    if ! ping -q -w 1 -c 1 $IP >/dev/null 2>&1; then
        echo "$IP unreachable, skipping..."
        continue
    fi

    HOSTNAME=$(curl -s "http://$IP/cm?cmnd="$(uriencode "Status 5") | jq -r '.StatusNET.Hostname' 2>/dev/null)

    if [ "x$HOSTNAME" != "x" ]; then
        echo -n "Getting hostname... "
        echo $HOSTNAME

        echo -n "Configure timezone... "
        curl -s "http://$IP/cm?cmnd="$(uriencode "Backlog Timezone 99; TimeDST 0,4,3,1,3,120; TimeSTD 0,4,10,1,3,60")
        echo
    fi
done

#Backlog IPAddress1 192.168.0.48; IPAddress2 192.168.0.1; IPAddress3 255.255.255.0; IPAddress4 192.168.0.4; restart 1
#Backlog IPAddress1 192.168.1.48; IPAddress2 192.168.1.1; IPAddress3 255.255.255.0; IPAddress4 192.168.0.4; restart 1
