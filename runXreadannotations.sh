#!/bin/bash
# Run docker image with X variant in detached mode
#!/bin/bash
# Run docker image with X variant in detached mode
CID=$(docker run -d -v $PWD/data:/data -v $PWD/scripts:/scripts -p 22 mbari/avedac-mbarivision-readannotations)
echo $CID
