#!/bin/bash -e

function check_presence() {
    set +e
    ping -c 5 $1 | grep ttl=
    is_present=$?
    if [ "x${is_present}" = "x0" ]; then
        mosquitto_pub -h openhab.master -t "presence/$2" -m "CLOSED"
    else
        mosquitto_pub -h openhab.master -t "presence/$2" -m "OPEN"
    fi
    set -e
}

check_presence "192.168.0.4" "router"
check_presence "192.168.0.224" "waldek"
check_presence "192.168.0.190" "aldona"
check_presence "192.168.0.114" "ann"
check_presence "192.168.0.129" "alice"
#check_presence "192.168.0.241" "waldek"
#check_presence "192.168.0.242" "aldona"
#check_presence "192.168.0.243" "ann"
#check_presence "192.168.0.244" "alice"
