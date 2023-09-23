#!/bin/bash
# iBeacon Scan by Radius Networks
# written by Michał Dobrzyński
# modified by iNode.pl 03.12.2017

declare -i cnt
declare -i dcnt
declare -i bcnt
declare -i tl
declare -i bl
declare -i bc
declare -i bt
declare -i DTYPE

if [[ $1 == "parse" ]]; then
  packet=""
  capturing=""
  count=0
  while read line; do
    count=$((count + 1))
    if [ "$capturing" ]; then
      if [[ $line =~ ^[0-9a-fA-F]{2}\ [0-9a-fA-F] ]]; then
        packet="$packet $line"
      else
        #echo RAW: $packet
        cnt=0
        dcnt=0
        bl=0
        bt=0
        bc=0
        np=""
        mp=""

        DTYPE=0

        for i in $packet; do
          if [[ "$cnt" -eq "13" ]]; then
            tl=$(echo "ibase=16; $i" | bc)
            #echo TL $tl
          fi
          if [[ "$cnt" -gt "13" ]]; then
            np+=$i
            if [[ "$bl" -eq "0" ]]; then
              if [[ "$DTYPE" -eq "130" ]]; then # iNode Energy Meter
                #echo DTYPE
                #echo $packet
                MAC=$(echo $packet | awk '{print $13$12$11$10$9$8}')
                #echo $mp

                #CONST
                HEX=$(echo $mp | awk '{ print $12$11 }')
                DEC=$(echo "ibase=16; $HEX" | bc)
                CONST=$DEC
                #echo CONST $CONST

                #WATY
                HEX=$(echo $mp | awk '{ print $6$5 }')
                DEC=$(echo "ibase=16; $HEX" | bc)
                #echo MINUTE POWER $DEC
                CALC=$(echo $DEC $CONST | awk '{ kw=(($1*60.0)/$2); printf"%1.3f\n",  kw  }')

                #IMPULSY
                HEXIM=$(echo $mp | awk '{ print $10$9$8$7 }')
                #echo TOTAL POWER $HEXIM
                DECIM=$(echo "ibase=16; $HEXIM" | bc)
                #echo TOTAL POWER $DECIM
                CALCIM=$(echo $DECIM $CONST | awk '{ kWh=($1/$2); printf"%0.4f\n", kWh  }')
                #echo CALCIM $CALCIM

                #set +e
                PREVCALC=$(cat /opt/openhab/prod/userdata/logs/CALC)
                PREVCALCIM=$(cat /opt/openhab/prod/userdata/logs/CALCIM)
                #set -e
                TODOMOTICZ=$(echo $CALCIM | sed -r 's/\.//g')

                #domoticz
                #/usr/bin/curl -s -o /dev/null  "http://10.0.0.2:8080/json.htm?type=command&param=udevice&idx=1&nvalue=0&svalue=$CALC;$TODOMOTICZ"

                #emoncms
                #/usr/bin/curl -s -o /dev/null  "http://emoncms.org/input/post.json?json={meterpower:$CALC}&apikey=XYZ"
                #/usr/bin/curl -s -o /dev/null  "http://emoncms.org/input/post.json?json={meterkwh:$CALCIM}&apikey=XYZ"

                #logowanie do lokalnego sysloga
                #logger POWER: $CALC W $CALCIM kWh -p local2.info
                #print na ekranie
                CALC_W=$(bc -l <<<"$CALC*1000.0")
                echo MAC: $MAC CONST: $CONST POWER: $CALC kW, $CALC_W W TOTAL: $CALCIM kWh | awk '{ print strftime("%Y-%m-%d %H:%M:%S"), $0; fflush(); }'
                echo mp: $mp | awk '{ print strftime("%Y-%m-%d %H:%M:%S"), $0; fflush(); }'
                if [ "${PREVCALC}" != "${CALC}" ]; then
                  echo "CALC:${PREVCALC}:${CALC}:${CALC_W}"
                  /usr/bin/mosquitto_pub -t myHome/PowerMeter_Current -m "${CALC}" -h "openhab.master"
                  /usr/bin/mosquitto_pub -t myHome/PowerMeter_Current_Watts -m "${CALC_W}" -h "openhab.master"
                fi
                if [ "${PREVCALCIM}" != "${CALCIM}" ]; then
                  echo "CALCIM:${PREVCALCIM}:${CALCIM}"
                  /usr/bin/mosquitto_pub -t myHome/PowerMeter_Total -m "${CALCIM}" -h "openhab.master"
                fi
                echo ${CALC} >/opt/openhab/prod/userdata/logs/CALC
                echo ${CALCIM} >/opt/openhab/prod/userdata/logs/CALCIM
                DTYPE=0
              fi

              if [[ "$dcnt" -lt "$tl" ]]; then
                bl=$(echo "ibase=16; $i" | bc)
                bcnt=0
                #echo BL $bl
                mp=$i" "
              fi
            else
              if [[ "$bcnt" -eq "0" ]]; then
                bc=$(echo "ibase=16; $i" | bc)
                #echo BC $bc
              fi
              if [[ "$bc" -eq "255" ]]; then
                if [[ "$bcnt" -eq "2" ]]; then
                  DTYPE=$(echo "ibase=16; $i" | bc)

                  #echo DTYPE $DTYPE
                fi
              fi

              bcnt=$bcnt+1

              bl=$bl-1

              mp+=$i" "
            fi
            dcnt=$dcnt+1
          fi
          cnt=$cnt+1
        done

        capturing=""
        packet=""
      fi
    fi

    if [ ! "$capturing" ]; then
      if [[ $line =~ ^\> ]]; then
        packet=$(echo $line | sed 's/^>.\(.*$\)/\1/')
        capturing=1
      fi
    fi
  done
else
  echo "Run powermeter.sh: hcitool lescan..."
  #  hcitool lescan --duplicates 1>/dev/null &
  #  DEVICE_NAME=$(rfkill list -o DEVICE -n | grep hci)
  DEVICE_NAME="hci0"
  # hciconfig $DEVICE_NAME down && hciconfig $DEVICE_NAME up && hcitool lescan --duplicates 1>/dev/null &
  hcitool lescan --duplicates 1>/dev/null &
  echo "Run powermeter.sh: device ${DEVICE_NAME}"

  if [ "$(pidof hcitool)" ]; then
    echo "hcidump --raw -i $DEVICE_NAME | $0 parse $1"
    hcidump --raw -i $DEVICE_NAME | $0 parse $1
    #hcidump --raw | $0 parse $1
  fi
fi
