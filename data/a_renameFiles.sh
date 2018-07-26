#!/bin/bash
i=1
ind=000001
p=f
for x in $p*.png ; do
  mv "$x" "f$ind.png"
  ((++i))
  ind=$i

  # $ind must have four digits
  for d in 10 100 1000 10000 100000 ; do
    if [ $i -lt $d ]
    then
      ind=0$ind
    fi
  done
done
i=1
ind=000001
for x in $p*.xml ; do
  mv "$x" "f$ind.xml"
  ((++i))
  ind=$i
  # $ind must have four digits
  for d in 10 100 1000 10000 100000 ; do
    if [ $i -lt $d ]
    then
      ind=0$ind
    fi
  done
done
