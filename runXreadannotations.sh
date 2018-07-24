#!/bin/bash
# Run docker image with X variant in detached mode
UN=mochoa
PWD=/Users/$UN/Documents/MBARI/avedac-mbarivision
CID=$(docker run -d -v $PWD/i2data:/i2data -v $PWD/scripts:/scripts -p 22 mbari/avedac-mbarivision-readannotations)
