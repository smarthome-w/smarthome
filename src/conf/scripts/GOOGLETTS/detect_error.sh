#!/bin/bash +e

PHRASE=$@

LINE_START=$(cat /openhab/userdata/logs/openhab.log | grep -n "SAY_START" | tail -n 1 | awk -F: '{ print$1 }')
echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - Line start: $LINE_START" | tee -a /openhab/userdata/logs/openhab.log

LINE_ERROR=$(cat /openhab/userdata/logs/openhab.log | grep -n "Error saying" | tail -n 1 | awk -F: '{ print$1 }')
echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - Line error: $LINE_ERROR" | tee -a /openhab/userdata/logs/openhab.log
if [[ "$LINE_ERROR" == "" ]]; then
    LINE_ERROR=0
fi

LINE_MISMATCH_SERVICE=$(cat /openhab/userdata/logs/openhab.log | grep -n "Missing service configuration" | tail -n 1 | awk -F: '{ print$1 }')
echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - Line mismatch: $LINE_MISMATCH_SERVICE" | tee -a /openhab/userdata/logs/openhab.log
if [[ "$LINE_MISMATCH_SERVICE" == "" ]]; then
    LINE_MISMATCH_SERVICE=0
fi

if [[ $LINE_ERROR -gt $LINE_START ]]; then
    ERROR_SAY="true"
fi

if [[ $LINE_MISMATCH_SERVICE -gt $LINE_START ]]; then
    ERROR_CONF="true"
fi

if [[ "$ERROR_CONF" == "true" ]]; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - Missing configuration detected" | tee -a /openhab/userdata/logs/openhab.log
    # Effectively change googletts key pitch 0<->1
    cat /openhab/conf/services/googletts.cfg | grep -i "pitch=0"
    PITCH=$?
    if [[ "$PITCH" == "0" ]]; then
        sed -i -r 's/^org.openhab.voice.googletts:pitch=.*/org.openhab.voice.googletts:pitch=1/g' /openhab/conf/services/googletts.cfg
        echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - pitch to 1" | tee -a /openhab/userdata/logs/openhab.log
    else
        sed -i -r 's/^org.openhab.voice.googletts:pitch=.*/org.openhab.voice.googletts:pitch=0/g' /openhab/conf/services/googletts.cfg
        echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - pitch to 0" | tee -a /openhab/userdata/logs/openhab.log
    fi
    sleep 2
fi

if [[ "$ERROR_SAY" == "true" ]] || [[ "$ERROR_CONF" == "true" ]]; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - GoogleError - \"$PHRASE\"" | tee -a /openhab/userdata/logs/openhab.log
    echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - Error saying - reset error indicator" | tee -a /openhab/userdata/logs/openhab.log
else
    echo "$(date '+%Y-%m-%d %H:%M:%S') GoogleTTS check - GoogleOK - \"$PHRASE\"" | tee -a /openhab/userdata/logs/openhab.log
fi
