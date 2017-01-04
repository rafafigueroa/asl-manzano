#!/bin/bash

# java -jar CWBQuery.jar -s "ICWMQ" -b "2016/09/01 00:00:00" \
# -d 86400 -h 137.227.224.97  -t dcc512
java_program_path_="/home/rafa/cwb"
seed_name_="XXDEV"
date_="2017/01/04"
time_="00:00:00"
# dcc512 - Outputs with best effort reconciliation to 512 byte mini-seed format with eliminated overlaps.
format_="dcc512"
ip_="136.177.121.21"

java \
    -jar $java_program_path_/CWBQuery.jar \
    -s $seed_name_ \
    -b $date_" "$time_ \
    -d 86400 \
    -h $ip_ \
    -t $format_
#    -ascii
