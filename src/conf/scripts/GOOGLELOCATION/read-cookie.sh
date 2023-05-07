#!/bin/bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

echo $SCRIPT_DIR

find /home/waldek/.mozilla/ -name cookies.sqlite -exec cp {} $SCRIPT_DIR/cookies.sqlite \;

echo "# HTTP Cookie File"
sqlite3 -separator '	' "$SCRIPT_DIR/cookies.sqlite" >$SCRIPT_DIR/google-cookie-auto.txt.sensitive <<-EOF
.mode tabs
.header off
select host,
case substr(host,1,1)='.' when 0 then 'FALSE' else 'TRUE' end,
path,
case isSecure when 0 then 'FALSE' else 'TRUE' end,
expiry,
name,
value
from moz_cookies
where host like '%google%';
EOF

scp $SCRIPT_DIR/google-cookie-auto.txt.sensitive root@192.168.0.4:/opt/openhab/conf/scripts/GOOGLELOCATION/google-cookie.txt.sensitive

rm $SCRIPT_DIR/cookies.sqlite
rm $SCRIPT_DIR/google-cookie-auto.txt.sensitive
