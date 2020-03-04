#!/bin/bash

#Constants defined here:
HWPANEL_IP_ADDRESS="192.168.88.243"
HWPANEL_URL="$HWPANEL_IP_ADDRESS/post"

while :
do

  #Get CPU cores load
  CORES_LOAD=$(sar -P ALL 1 1 | grep -v "all" | grep -v "Average" | tail -n 9 | head -n 8 |  awk '{print 100-$9}')

  CORE01=$(echo "$CORES_LOAD" | head -n 1)
  CORE02=$(echo "$CORES_LOAD" | head -n 2 | tail -n 1)
  CORE03=$(echo "$CORES_LOAD" | head -n 3 | tail -n 1)
  CORE04=$(echo "$CORES_LOAD" | head -n 4 | tail -n 1)
  CORE05=$(echo "$CORES_LOAD" | head -n 5 | tail -n 1)
  CORE06=$(echo "$CORES_LOAD" | head -n 6 | tail -n 1)
  CORE07=$(echo "$CORES_LOAD" | head -n 7 | tail -n 1)
  CORE08=$(echo "$CORES_LOAD" | head -n 8 | tail -n 1)


  #Used memory in percents
  MEMORY_LOAD=$(echo -n "$(free -h | grep "Mem" | awk '{print ($2-$7)/$2 * 100}')")

  #Used hdd in percents
  HDD_LOAD=$(df -k | grep "ubuntu--vg-root" | awk '{print $5}' | tr -d '%')


  #Preparation of request body
  JSON_STRING=$( jq -n \
                    --arg core01 "${CORE01%.*}" \
                    --arg core02 "${CORE02%.*}" \
                    --arg core03 "${CORE03%.*}" \
                    --arg core04 "${CORE04%.*}" \
                    --arg core05 "${CORE05%.*}" \
                    --arg core06 "${CORE06%.*}" \
                    --arg core07 "${CORE07%.*}" \
                    --arg core08 "${CORE08%.*}" \
                    --arg mem_load "${MEMORY_LOAD%.*}" \
                    --arg hdd_load "${HDD_LOAD%.*}" \
                    '{CORE01: 75, CORE02: 75, CORE03: 75, CORE04: 75,
                      CORE05: 75, CORE06: 75, CORE07: 75, CORE08: 75,
                      MEMORY_LOAD: 75, HDD_LOAD: 75}' )
  clear;
  echo "$JSON_STRING" | jq .

  curl -vvv -X POST "$HWPANEL_URL" --header 'Content-Type: application/json' --data-raw "$JSON_STRING"

  sleep 1

done
