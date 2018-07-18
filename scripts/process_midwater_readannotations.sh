#!/bin/bash
code/src/readAnnotations.C --in=raster:/tmp/data/f#.png --out=raster:/tmp/data/all_results/ --input-frames=1-898@1  --output-frames=1-898@1 --mbari-save-results --mbari-save-output --mbari-save-events-xml=results.xml
