#Quick Curver
This is an OpenGL accelerated Qt/C++ implementation of the famous game "Achtung die Kurve".

![alt tag](http://i.imgur.com/IGa3NwA.png)

Please note that this project is yet in development and hasn't reached a stable version for now. This means that you could experience bugs, when playing this game.

#Features
* Material Design
* Local Multiplayer
* Online Multiplayer (still in development)
* Bots
* Items


#Installation
##Compiling from source
###Dependencies
First make sure, that you have the required dependencies of QuickCurver installed. These are:

* g++ (You may use an alternative C++ compiler, but things will get slightly more difficult)
* make
* At least Qt 5.7
* The following Qt Modules (package name depends on distribution, Note: These modules are typically distributed in 3 different packages which are probably called something like qtbase, qtdeclarative and qtsvg):
	- Qt5Core
	- Qt5Gui
	- Qt5Quick
	- Qt5Qml
	- Qt5Svg
	- Qt5Network

###Build instructions
####Cloning using SSH
This is the preferred way, simply execute:
```{r, engine='bash', count_lines}
git clone --recursive git@github.com:Magnus2552/quickcurver.git
cd quickcurver
./build.sh # You may have to fix the permissions first with chmod +x ./build.sh
./QuickCurver # starting the executable may vary on OSs other than Linux
```

####Cloning using HTTPS
If you do not have setup SSH keys on your machine, you can alternatively run the following commands:
```{r, engine='bash', count_lines}
git clone https://github.com/Magnus2552/quickcurver.git
cd quickcurver
./httpsClonePatch.sh # You may have to fix the permissions first with chmod +x ./httpsClonePatch.sh
git submodule init
git submodule update
./build.sh # You may have to fix the permissions first with chmod +x ./build.sh
./QuickCurver # starting the executable may vary on OSs other than Linux
```

In general prefer to use the script `./build.sh`. However, if the script fails due to an **unexplainable reason**, you may want to run `./build.sh -f`. If this still fails, try the following manually instead of `./build.sh`:
```{r, engine='bash', count_lines}
mkdir build; cd build
qmake ..
make
```

###Updating
If you built QuickCurver from source you can simply update by running the following script:
```{r, engine='bash', count_lines}
./update.sh # You may have to fix the permissions first with chmod +x ./update.sh
```

##Installing compiled binaries
Coming soon!

#Multiplayer
To play multiplayer, the host has to make at least one player an "Online Player" by clicking on that player in the Playerselector screen and using the drop down menu. Once a slot is open to join, the host has to publish his IP to the client. Note that the program automatically prints out the IP, assuming you have started it CLI. If you are not in the same local network, the host has to use [Port Forwarding](https://en.wikipedia.org/wiki/Port_forwarding) to make his device available to the internet. When the client receives the host's IP, he has to start the program, click on the "Join" Icon on the top right and enter the IP. If the join request was successfull, you will see "Joined, waiting for host to start...", otherwise the connection will timeout after a few seconds.

#Troubleshooting
##The drawing does not get updated, or only rarely
Use one of the following commands to start QuickCurver instead:
```{r, engine='bash', count_lines}
QSG_RENDER_LOOP=basic ./QuickCurver #prefer this one
QSG_RENDER_LOOP=windows ./QuickCurver #alternatively use this command
```

#Acknowledgements
QuickCurver uses [qml-material](https://github.com/papyros/qml-material), which is a nice QML Material Design library.
