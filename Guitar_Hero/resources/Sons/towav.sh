#!/bin/bash

SAVEIF=$IFS
IFS=$(echo -en "\n\b")

for file in $(ls *ogg)
do
  name=${file%%.ogg}
  ffmpeg -i "$name.ogg" "$name.wav"
done


IFS=$SAVEIFS
