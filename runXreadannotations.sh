#!/bin/bash
# Run docker image with X variant in detached mode
<<<<<<< HEAD
UN=Gray
PWD=/Users/$UN/Documents/MBARI/avedac-mbarivision
CID=$(docker run -d -v $PWD/data:/data -v $PWD/scripts:/scripts -p 22 mbari/avedac-mbarivision-readannotations)
=======
#!/bin/bash
# Run docker image with X variant in detached mode
CID=$(docker run -d -v /Users/Gray/Documents/MBARI/avedac-mbarivision/data:/data -v /Users/Gray/Documents/MBARI/avedac-mbarivision/scripts:/scripts -p 22 mbari/avedac-mbarivision-readannotations)
>>>>>>> ac7cf733ebdb9fc2967f780e513ea6a5249a7f24
echo $CID
