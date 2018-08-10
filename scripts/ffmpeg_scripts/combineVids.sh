#!/bin/bash

# Combine 2 Videos

algo1=hough_top_vs_kalman_bot
algo2=kalman_n_hough_vs_og
newExt=Output

# If both videos are not the same size
#   - Rescale one of the videos to be the same as the other
# ---------------------------------------------------------
# 1) ffmpeg
# 2) -i ___.mp4             (File)
# 3) -vf                    (Video Format?)
# 4) -scale=Width x Height  (Rescale to new size)
# 5) ____.mp4               (New/Output File)
# ---------------------------------------------------------
#ffmpeg -i $algo2.mp4 -vf scale=2048:2160 $algo2$newExt.mp4

# ---------------------------------------------------------
# 1)    ffmpeg
# 2/3)  -i ___.mp4                  (File)
# 4)    -crf                        (Video Quality - Lower the better, 0 = Preserves Quality, Higher = Bad Quality)
# 6)    -filter_complex _stack      (hstack = Horizontal videos when combined, vstack = Vertical videos)
# 7)    ____.mp4                        (New/Output File)
# ---------------------------------------------------------
ffmpeg -i $algo1.mp4 -i $algo2.mp4 -crf 0 -preset ultrafast -filter_complex hstack compare_all_algo.mp4
