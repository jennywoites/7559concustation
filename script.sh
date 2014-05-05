#!/bin/bash

IPCS_M=`ipcs -m | egrep "0x[0-9a-f]+ [0-9]+ " | grep " 644" | grep -v "dest" | cut -f2 -d" "`
IPCS_S=`ipcs -s | egrep "0x[0-9a-f]+ [0-9]+" | grep " 666" | grep -v "dest" | cut -f2 -d" "`

for id in $IPCS_M; do
  ipcrm -m $id;
  echo $id;
done
for id in $IPCS_S; do
  ipcrm -s $id;
  echo $id;
done

rm log.jem
