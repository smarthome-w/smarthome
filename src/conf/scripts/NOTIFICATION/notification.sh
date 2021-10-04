#!/usr/bin/env bash

MY_PATH="$(dirname "$0")"
source "${MY_PATH}/notification.sensitive"

#type, itemName, itemValue, currentTime

TYPE=$1
ITEM_NAME=$2
ITEM_VALUE=$3
CURRENT_TIME=$4

#OPEN jest
#Aldona_AtHome
#Aldona_AtDistrict
#Aldona_AtSchool

TITLE=$ITEM_NAME
MSG="$TYPE $ITEM_NAME $ITEM_VALUE $CURRENT_TIME"
PRIORITY=-2

if [[ "$TYPE" == "presence" ]]; then
    IFS='_' tokens=($ITEM_NAME)
    PERSON_NAME=$tokens
    MSG=$CURRENT_TIME
    PRIORITY=1

    if [[ "$@" == *"Ann"* ]]; then
        PERSON_NAME="Ania"
    fi

    if [[ "$@" == *"Alice"* ]]; then
        PERSON_NAME="Alicja"
    fi

    if [[ "$@" == *"AtHome"* ]]; then
        if [[ "$ITEM_VALUE" == "OPEN" ]]; then
            TITLE="$PERSON_NAME w domu"
        else
            TITLE="$PERSON_NAME poza domem"
        fi
    fi
    if [[ "$@" == *"AtSchool"* ]]; then
        if [[ "$ITEM_VALUE" == "OPEN" ]]; then
            TITLE="$PERSON_NAME w szkole"
        else
            TITLE="$PERSON_NAME poza szkołą"
        fi
    fi
    if [[ "$@" == *"AtDistrict"* ]]; then
        if [[ "$ITEM_VALUE" == "OPEN" ]]; then
            TITLE="$PERSON_NAME w dzielnicy"
        else
            TITLE="$PERSON_NAME poza dzielnicą"
        fi
    fi
fi

if [[ "$TYPE" == "garage" ]]; then
    MSG=$CURRENT_TIME
    PRIORITY=1
    if [[ "$ITEM_VALUE" == "OPEN" ]]; then
        TITLE="Garaż otwarty"
    else
        TITLE="Garaż zamknięty"
    fi
fi

if [[ "$TYPE" == "error" ]]; then
    MSG="$ITEM_NAME $CURRENT_TIME"
    if [[ "$ITEM_VALUE" == "ON" ]]; then
        TITLE="Błąd"
    else
        TITLE="OK"
    fi
fi

if [[ "$TYPE" == "alarm" ]]; then
    MSG="$ITEM_NAME $CURRENT_TIME"
    PRIORITY=1
    if [[ "$ITEM_VALUE" == "OPEN" ]]; then
        TITLE="Otwarte"
    else
        TITLE="Zamknięte"
    fi
fi

if [[ "$TYPE" == "information" ]]; then
    MSG="$ITEM_NAME $CURRENT_TIME"
    TITLE="$ITEM_VALUE"

    if [[ "$@" == *"MainLock"* ]]; then
        PRIORITY=1
        if [[ "$ITEM_VALUE" == "OPEN" ]]; then
            TITLE="Zamek otwarty"
        else
            TITLE="Zamek zamknięty"
        fi
        MSG=$CURRENT_TIME
    fi

    if [[ "$@" == *"MainDoor"* ]]; then
        PRIORITY=1
        if [[ "$ITEM_VALUE" == "OPEN" ]]; then
            TITLE="Drzwi otwarte"
        else
            TITLE="Drzwi zamknięte"
        fi
        MSG=$CURRENT_TIME
    fi
fi

curl -s \
    --form-string "token=${PUSHOVER_APPLICATION}" \
    --form-string "user=${PUSHOVER_USER_KEY}" \
    --form-string "message=$MSG" \
    --form-string "title=$TITLE" \
    --form-string "priority=$PRIORITY" \
    https://api.pushover.net/1/messages.json
