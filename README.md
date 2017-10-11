# Quick Curver
This is an OpenGL accelerated Qt/C++ implementation of the famous game "Achtung die Kurve".

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

* g++ with C++ 14 support (You may use an alternative C++ compiler, but things will get slightly more difficult)
* make
* At least Qt 5.8
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
# You may have to run git submodule update --init, if you didn't clone recursively
cd quickcurver
mkdir build
cd build
qmake ..
make
```

To start QuickCurver you need to run the built executable in the build directory, for example on Linux: `./QuickCurver`

## Installing compiled binaries
Coming soon!

# Multiplayer
To play multiplayer, the host starts an instance and shares the port that QuickCurver is running on. The client then just has to connect to this port on the host's ip address.
If you are not in the same local network, the host most likely has to use [Port Forwarding](https://en.wikipedia.org/wiki/Port_forwarding) to make his device available to the internet.
If a firewall is the problem, you might also want to take a look at [Hole Punching](https://en.wikipedia.org/wiki/Hole_punching_(networking)).

If network performance isn't good, the Server can tweak the "network block" value in the settings, which causes data to be sent less frequently which may improve the network performance at the cost of update frequency. (A higher value means worse quality, but better network performance)

# Troubleshooting
## The drawing does not get updated, or only rarely
Use one of the following commands to start QuickCurver instead:
```bash
QSG_RENDER_LOOP=basic ./QuickCurver #prefer this one
QSG_RENDER_LOOP=windows ./QuickCurver #alternatively use this command
```

# Acknowledgements
QuickCurver uses [fluid](https://github.com/lirios/fluid), which is a neat QML Material Design library.
