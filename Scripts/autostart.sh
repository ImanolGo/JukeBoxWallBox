#!/bin/bash


echo '-> Jukebox App Script'

echo '-> Starting Jukebox Application'


cd ~/Desktop/Jukebox/OpenFrameworks/JukeboxPlayerApp/bin
ret=1
while [ $ret -ne 0 ]; do
    ./JukeboxPlayerApp
    ret=$?
    sleep 60
done
