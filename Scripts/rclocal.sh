#!/bin/sh -e
#
# rc.local
#
# This script is executed at the end of each multiuser runlevel.
# Make sure that the script will "exit 0" on success or any other
# value on error.
#
# In order to enable or disable this script just change the execution
# bits.
#
# By default this script does nothing.

echo "runninng Jimmy Choo" > /tmp/rc_test.txt


/home/pi/Jimmy-Choo-Shop-Window/Scripts/network-monitor.sh &

(sleep 15 && /home/pi/Desktop/Jukebox/Scripts/autostart.sh) &

exit 0