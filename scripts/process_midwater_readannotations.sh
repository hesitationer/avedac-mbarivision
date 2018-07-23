#!/bin/bash
export DISPLAY=docker.for.mac.localhost:0
export AVED_BIN=/usr/local/bin
readAnnotations --in=raster:/data/f#.png \
--out=raster:/data/all_results/ \
--input-frames=1-898@1 \
--output-frames=1-898@1 \
--mbari-save-results \
--mbari-save-output \
--mbari-save-events-xml=/data/all_results/results.xml \
--mbari-display-results --logverb=Info \
--mbari-rescale-display=320x240 \
--mbari-tracking-mode=Hough \
--mbari-mark-interesting=Outline \
--mbari-segment-algorithm-input-image=Luminance \
--mbari-min-event-frames=3 --mbari-color-space=RGB \
--mbari-se-size=4 \
--mbari-cache-size=120 \
--mbari-segment-algorithm=Best --mbari-rescale-display=320x240 \
--mbari-min-event-frames=1 \
--mbari-segment-graph-parameters=0.75,500,250 \
--mbari-segment-adaptive-parameters=2,2 --mbari-dynamic-mask=true
