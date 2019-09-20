#!/bin/bash

for num in {1..228}
do
  qry="use OpenHAB; select max(time) from Item$num;"
  echo $qry
  qry_res=`echo $qry | sudo mysql -u root`
  if [[ $qry_res = *2019-* ]]; then
    echo "$num ok"
  else
    echo "$num not ok: $qry_res"
    qry="use OpenHAB; delete from items where ItemId = $num;"
    echo "qry_del: $qry"
    qry="use OpenHAB; drop table Item$num;"
    echo "qry_drop: $qry"
  fi
done
