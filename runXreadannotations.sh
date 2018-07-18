#!/bin/bash
# Run docker image with X variant in detached mode
#!/bin/bash
# Run docker image with X variant in detached mode
CID=$(docker run -d -v /Users/Gray/Documents/MBARI/avedac-mbarivision/data:/data -v /Users/Gray/Documents/MBARI/avedac-mbarivision/scripts:/scripts -p 22 mbari/avedac-mbarivision-readannotations)
echo $CID
