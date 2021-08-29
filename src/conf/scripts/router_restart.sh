#!/bin/ash
set +e
/bin/ping -c 5 192.168.0.4
internal_fail=$?
set -e
set +e
/bin/ping -c 5 google.com
external_fail=$?
set -e
echo "internal_fail=$internal_fail"
echo "external_fail=$external_fail"

if [ $external_fail -eq 1 ]; then
    echo "$(date) external_fail=$external_fail, internal_fail=$internal_fail" >>/root/restart.log
    sleep 5
    reboot
elif [ $internal_fail -eq 1 ]; then
    echo "$(date) external_fail=$external_fail, internal_fail=$internal_fail" >>/root/restart.log
    sleep 5
    reboot
else
    echo "$(date) check ok"
fi
