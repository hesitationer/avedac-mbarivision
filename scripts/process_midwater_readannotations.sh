#!/bin/bash
export AVED_BIN=/usr/local/bin
readAnnotations --in=raster:/data/f#.png --out=raster:/data/all_results/ --input-frames=1-898@1 --output-frames=1-898@1 --mbari-save-results --mbari-save-output --mbari-save-events-xml=results.xml
