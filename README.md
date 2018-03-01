# Quick Curver
This is an OpenGL accelerated Qt/C++ implementation of the famous game "Achtung die Kurve".

[![Build Status](https://travis-ci.org/magnus-gross/quickcurver.svg?branch=develop)](https://travis-ci.org/magnus-gross/quickcurver)

![alt tag](http://i.imgur.com/IGa3NwA.png)

Please note that this project is still in development. This means that you could experience bugs, while playing the game. Please open an issue if you encounter a bug.

# Features
* Material Design
* Local Multiplayer
* Online Multiplayer
* Bots
* Items


# Installation
## Compiling from source

Note: Arch Linux users can simply install [quickcurver-git](https://aur.archlinux.org/packages/quickcurver-git) from the AUR:

```bash
pacaur -S quickcurver-git
quickcurver
```

### Dependencies
First make sure, that you have the required dependencies of QuickCurver installed. These are:

* A C++ compiler with C++14 support
* make
* At least Qt 5.10
* The following Qt Modules (in the parantheses there is an example how the package could be called for your distro (this depends on the distro!)):
	- Qt Core (qt5-base)
	- Qt GUI (qt5-base)
	- Qt Quick (qt5-declarative)
	- Qt QML (qt5-declarative)
	- Qt SVG (qt5-svg)
	- Qt Network (qt5-base)
	- Qt Quick Controls 2 (qt5-quickcontrols2)
	- Qt Graphical Effects (qt5-graphicaleffects)

### Build instructions
Run the following commands:
```bash
git clone --recursive git@github.com:magnus-gross/quickcurver.git
# Alternatively clone with HTTPS, if you don't have SSH keys
# If you forgot to clone with --recursive, just run git submodule update --init
cd quickcurver
mkdir build
cd build
qmake ..
make
```

To start QuickCurver you need to run the built executable in the build/src directory, for example on Linux run: `src/QuickCurver`

## Installing compiled binaries
Coming soon!

# Multiplayer
To play multiplayer, the host starts an instance and shares the port that QuickCurver is running on. The client then just has to connect to this port on the host's ip address.
If you are not in the same local network, the host most likely has to use [Port Forwarding](https://en.wikipedia.org/wiki/Port_forwarding) to make his device available to the internet.
If a firewall is the problem, you might also want to take a look at [Hole Punching](https://en.wikipedia.org/wiki/Hole_punching_(networking)).

If network performance isn't good, the Server can tweak the "Network update rate" value in the settings, which causes data to be sent less frequently which may improve the network performance at the cost of update frequency. (A higher value means worse quality, but better network performance)

# Acknowledgements
QuickCurver uses [fluid](https://github.com/lirios/fluid), which is a neat QML Material Design library.
