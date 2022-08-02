#!/bin/bash +e

PHRASE=$@
cat /openhab/userdata/logs/openhab.log | grep -i "Error saying" | tail -n 1 | grep "$PHRASE"
RET_VAL=$?
if [[ "$RET_VAL" == "0" ]]; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleError - \"$PHRASE\"" | tee -a /openhab/userdata/logs/openhab.log
    echo "$(date '+%Y-%m-%d %H:%M:%S') error saying - reset error indicator" | tee -a /openhab/userdata/logs/openhab.log
else
    echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleOK - \"$PHRASE\"" | tee -a /openhab/userdata/logs/openhab.log
fi
