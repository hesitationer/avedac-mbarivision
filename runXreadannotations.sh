#!/bin/bash
# Run docker image with X variant in detached mode
CID=$(docker run -d -p 22 readannotations -v $PWD/data:/data)
