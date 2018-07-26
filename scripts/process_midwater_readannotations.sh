#!/bin/bash
export DISPLAY=docker.for.mac.localhost:0
export AVED_BIN=/usr/local/bin
readAnnotations --in=raster:/data/f#.png \
--out=raster:/data/all_results/ \
--input-frames=1-5@1 \
--output-frames=1-5@1 \
--mbari-save-results \
--mbari-save-output \
--mbari-save-original-frame-spec \
--mbari-save-events-xml=/data/all_results/results.xml \
<<<<<<< HEAD
--mbari-display-results --logverb=Info \
--mbari-rescale-display=400x300 \
=======
--mbari-display-results --logverb=Debug \
--mbari-rescale-display=960x540 \
>>>>>>> 51c0b0b994ff7d68ff48d4b9b0c2a3944d93b6a5
--mbari-tracking-mode=Hough \
--mbari-mark-interesting=Outline \
--mbari-segment-algorithm-input-image=Luminance \
--mbari-se-size=10 \
--mbari-cache-size=120 \
--mbari-segment-algorithm=Best \
<<<<<<< HEAD
--mbari-min-event-area=100 \
--mbari-segment-graph-parameters=0.75,500,250 \
--mbari-segment-adaptive-parameters=2,2 --mbari-dynamic-mask=true
=======
--mbari-min-event-frames=1 \
--mbari-min-event-area=100 \
--mbari-segment-graph-parameters=0.75,500,250 \
--mbari-dynamic-mask=true
>>>>>>> 51c0b0b994ff7d68ff48d4b9b0c2a3944d93b6a5
