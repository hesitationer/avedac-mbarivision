#!/bin/bash

# Converts all .pnm (Results) to .png

mkdir pngs
i=0
ind=000001
p=results

# Finds all .pnm files
for x in $p*.pnm + 1; do

  # $ind must have four digits
  # Ensures there are 6 digits added to the 
  for d in 10 100 1000 10000 100000 ; do
    if [ $i -lt $d ]
    then
      ind=0$ind
    fi
  done

  # Saves the .pnm to a .png in the pngs/
  ffmpeg -i $p$ind.pnm pngs/r$ind.png

  ((++i))
  ind=$i

done
