#!/bin/bash
mkdir pngs
i=0
ind=000001
p=results
for x in $p*.pnm + 1; do

  # $ind must have four digits
  for d in 10 100 1000 10000 100000 ; do
    if [ $i -lt $d ]
    then
      ind=0$ind
    fi
  done

  ffmpeg -i $p$ind.pnm pngs/r$ind.png

  ((++i))
  ind=$i

done
