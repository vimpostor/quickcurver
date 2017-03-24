#!/usr/bin/env bash

NUMBEROFPLAYERS=$1
if [ "$NUMBEROFPLAYERS" = "" ]; then
	NUMBEROFPLAYERS=2
fi
ANIMATIONDURATION=0.4
BUILDDIRECTORY='../build'
if ! [ -d $BUILDDIRECTORY ]; then
	echo 'Please create a build directory first and build the application'
	exit 1
fi
cd $BUILDDIRECTORY
if ! [ -f './QuickCurver' ]; then
	echo 'Please compile first'
	exit 1
fi
./QuickCurver 2>&1 | xargs -0 echo -e '\e[0;32m' & # Start server (colored green)
sleep $ANIMATIONDURATION # Wait for application to start
SWID=$(xdotool search 'Quick Curver'| head -1) # Window ID of server
xdotool key 'Ctrl+c' key Tab key Tab key space # start the server
xdotool key 'Ctrl+c' # Copy ip address

# Choose one player to be an online player
xdotool mousemove --window $SWID 150 230 click 1
sleep $ANIMATIONDURATION
xdotool mousemove --window $SWID 660 470 click 1
sleep $ANIMATIONDURATION
xdotool mousemove --window $SWID 660 500 click 1 mousemove --window $SWID 700 50 click 1
# Create the rest of players
for (( i = 2; i < NUMBEROFPLAYERS; i++ )); do
	xdotool key "Ctrl+Shift+n"
done

./QuickCurver 2>&1 | xargs -0 echo -e '\e[0;31m' & # Start Client (colored red)
sleep $ANIMATIONDURATION # Wait for application to start
CWID=$(xdotool search 'Quick Curver <2>'| head -1) # Window ID of client
xdotool key 'Ctrl+j' key 'Ctrl+v' # insert Ip address
xdotool key Tab key Tab # Tab to the name field
xdotool type 'Unit Test'
xdotool key Tab # go to the ready checkbox
xdotool key space
xdotool key Tab # Go to the join button
xdotool key space # click the join button
sleep $ANIMATIONDURATION # wait for the join request to process

xdotool windowactivate $SWID # Switch back to server
xdotool key Tab # Tab to the start button
# xdotool key space # and start the game

echo 'Finished!'
exit 0
