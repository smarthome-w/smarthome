#!/usr/bin/env bash

MY_PATH="$(dirname "$0")"
source "${MY_PATH}/slack.sensitive"

CHANNEL="#home"

if [[ "$@" == *":interrobang:"* ]]; then
  CHANNEL="#alerton"
fi

if [[ "$@" == *":ok:"* ]]; then
  CHANNEL="#alertoff"
fi

if [[ "$@" == *":car:"* ]]; then
  CHANNEL="#garage"
fi

if [[ "$@" == *":articulated_lorry:"* ]]; then
  CHANNEL="#garage"
fi

if [[ "$@" == *":house:"* ]]; then
  CHANNEL="#presence"
fi

if [[ "$@" == *":cityscape:"* ]]; then
  CHANNEL="#presence"
fi

ENCODED_PART=""
ENCODED_PART="token=${SLACK_TOKEN}"
ENCODED_PART+="&as_user=false"
ENCODED_PART+="&username=OpenHAB"
ENCODED_PART+="&channel=${CHANNEL}"
ENCODED_PART+="&text=$@"

echo ${ENCODED_PART}

curl -X POST --insecure -H "Content-Type: application/x-www-form-urlencoded" \
  https://slack.com/api/chat.postMessage --data "${ENCODED_PART}"
