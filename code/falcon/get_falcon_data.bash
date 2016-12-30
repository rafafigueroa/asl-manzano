#!/bin/bash

source falcon.conf
# user_="x"
# pw_="x"
# ip_="http://x.x.x.x"
# port_=0000

# copy data from the website into website_data directory
# only minute data gets copied
# -------------------------------------------------------- #
wget -r -np --user $user_ --password $pw_ -A csv \
    -k -nH -nd -Pwebsite_data -e robots=off \
    $ip_:$port_/data/minute

# get the file names from the output directory
# -------------------------------------------------------- #
csv_files="$(find $PWD -name *.csv -type f)"

# run the falcon program which takes the falcon formatted
# csv files (with a special header) and creates binary
# files which can be streamed to other processes
# -------------------------------------------------------- #
././build/falcon $csv_files