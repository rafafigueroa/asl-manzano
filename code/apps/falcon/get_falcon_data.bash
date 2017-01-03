#!/bin/bash

# directory locations, done in the cmake configuration
# can be skipped if wanted, just provide the values here, for example:
# dir_website_data=/home/rafa/asl-manzano/code/apps/falcon/website_data
# dir_falcon_config=/home/rafa/asl-manzano/runtime_config
# ------------------------------------------------------------------- #
source falcon_dirs.conf

# user_="x"
# pw_="x"
# ip_="http://x.x.x.x"
# port_=0000
# ------------------------------------------------------------------- #
echo -e "\e[1m ##### reading configuration file ##### \e[0m"
source $dir_falcon_config/falcon.conf

echo -e "\e[1m ##### remove old website_data files ##### \e[0m"
# ------------------------------------------------------------------- #
if [[ -n ${dir_website_data} ]]; then
    rm $dir_website_data/*
fi

# copy data from the website into website_data directory
# only minute data gets copied
echo -e "\e[1m ##### getting data from falcon ##### \e[0m"
# ------------------------------------------------------------------- #
wget -r -np --user $user_ --password $pw_ -A csv \
    -k -nH -nd -P$dir_website_data -e robots=off \
    $ip_:$port_/data/minute

# get the file names from the output directory
# ------------------------------------------------------------------- #
csv_files="$(find $dir_website_data -name *.csv -type f)"

# run the falcon program which takes the falcon formatted
# csv files (with a special header) and creates binary
# files which can be streamed to other processes
echo -e "\e[1m ##### running falcon script ##### \e[0m"
# ------------------------------------------------------------------- #
dir_falcon_app=./build
./$dir_falcon_app/falcon $csv_files
