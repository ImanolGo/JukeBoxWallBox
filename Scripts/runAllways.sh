#!/bin/bash

#-----------------------------------------------------------
#
# start Jimmy Choo Application
#
#-----------------------------------------------------------

echo '-> Jukebox App Script'

dir=$(dirname $0)
cd $dir

echo '-> Starting Jukebox Application'

PROCESS=JukeboxPlayerAp
export DISPLAY=:0.0
cd ~/Jukebox/OpenFrameworks/JukeboxPlayerApp/bin

while true; do
	if pgrep $PROCESS
		then
			continue
		else
			echo '-> Starting JukeboxPlayerAp process...'
			./JukeboxPlayerApp &
	fi
	sleep 20
done
