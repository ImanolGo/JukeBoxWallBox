#!/bin/bash


echo '-> Jukebox App Script'

echo '-> Starting Jukebox Application'
export DISPLAY=:0.0

cd /home/pi/Jukebox/OpenFrameworks/JukeboxPlayerApp/bin
ret=1
while [ $ret -ne 0 ]; do
    ./JukeboxPlayerApp
    sleep 20
    ret=$?
    sleep 60
done
