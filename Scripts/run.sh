#!/bin/bash


echo '-> Jukebox App Script'

echo '-> Starting Jukebox Application'

export DISPLAY=:0.0

cd /home/pi/Jukebox/OpenFrameworks/JukeboxPlayerApp/bin
./JukeboxPlayerApp

