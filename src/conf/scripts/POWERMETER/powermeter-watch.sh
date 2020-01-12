#!/bin/bash

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

while [ "1" = "1" ]; do
    sleep 10
    echo "$(date) Trying to run ${DIR}/powermeter.sh"
    ${DIR}/powermeter.sh
done
