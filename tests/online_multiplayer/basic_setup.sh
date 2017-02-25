#!/usr/bin/env bash

ANIMATIONDURATION=0.4
BUILDDIRECTORY='../../build'
if ! [ -d $BUILDDIRECTORY ]; then
	echo 'Please create a build directory first and build the application'
	exit 1
fi
cd $BUILDDIRECTORY
./QuickCurver & # Start server
sleep $ANIMATIONDURATION # Wait for application to start
SWID=$(xdotool search 'Quick Curver'| head -1) # Window ID of server
xdotool key 'Ctrl+c' # Copy ip address

# Choose one player to be an online player
xdotool mousemove --window $SWID 150 240 click 1
sleep $ANIMATIONDURATION
xdotool mousemove --window $SWID 700 520 click 1
sleep $ANIMATIONDURATION
xdotool mousemove --window $SWID 700 565 click 1 mousemove --window $SWID 700 50 click 1

./QuickCurver & # Start Client
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
xdotool key space # and start the game

echo 'Finished!'
exit 0
