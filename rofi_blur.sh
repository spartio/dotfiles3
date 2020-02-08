#!/bin/sh

TMP=`mktemp -u`

ffmpeg \
-video_size 1366x768 \
-f x11grab -i $DISPLAY \
-vf avgblur=5:15:5 \
-vframes 1 \
-f image2pipe -vcodec png \
-preset ultrafast \
$TMP > /dev/null

feh -F $TMP &
PID=$!

rofi -modi combi -show combi -combi-modi drun,run

kill $PID
rm $TMP