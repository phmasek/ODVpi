#!/bin/bash
# Purpose: Runscript for RealtimeService
#		   to record data for a certain
#		   amount of iterations.
# Author: Philip Masek
# 
# Flags:
# -o [Occupy]
# -i [Iterations to run the RT code]
# -f [Frequency]
# 
# ---------------------------------------



usage() { echo "Usage: $0 [-o String (Output file)] [-i int (Iterations)]" 1>&2; exit 1; }

while getopts ":o:i:" opt; do
    case "${opt}" in
        o)
            o=${OPTARG}
            ;;
        i)
            i=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z "${o}" ] || [ -z "${i}" ]; then
    usage
fi

FILENAME=${o}
[ ! -d ./data ] && mkdir -p ./data
[ ! -d ./data/$FILENAME ] && mkdir -p ./data/$FILENAME
for ((x=1;x<=$i;x++))
do
	grabserial -d "/dev/ttyUSB0" -b 115200 -w 8 -p N -s 1 -o ./data/$FILENAME/${x}-${FILENAME}.log  --quitpat=end

	screen -d -m ./commit_data.sh 

        echo "${x} iteration(s) done";
done


