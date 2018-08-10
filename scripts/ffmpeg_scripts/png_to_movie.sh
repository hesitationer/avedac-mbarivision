#!/bin/bash
# Converts all .png in current directory to a .mp4
ffmpeg -framerate 2/3 -i r%6d.png -crf 0 output.mp4
