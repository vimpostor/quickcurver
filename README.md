# Quick Curver
This is a hardware accelerated implementation of the famous game "Achtung die Kurve", written in modern C++20 and Qt 6.

[![Continuous Integration](https://github.com/vimpostor/quickcurver/actions/workflows/ci.yml/badge.svg)](https://github.com/vimpostor/quickcurver/actions/workflows/ci.yml)

![screenshot](https://user-images.githubusercontent.com/21310755/93923083-2ed2e300-fd13-11ea-86f1-d79bc09ce96a.png)

# Features
* Material Design
* Local Multiplayer
* Online Multiplayer
* Bots
* Items


# Installation
## Compiling from source

Note: For Arch Linux users there is an [AUR package](https://aur.archlinux.org/packages/quickcurver-git), for Nix users there is a [flake](flake.nix) available.

### Dependencies
First make sure, that you have the required dependencies of QuickCurver installed. These are:

* A C++ compiler with C++23 support
* The latest Qt
* The following Qt Modules (in the parentheses there is an example how the package could be called for your distro (this depends on the distro!)):
	- Qt Core (qt6-base)
	- Qt GUI (qt6-base)
	- Qt Quick (qt6-declarative)
	- Qt QML (qt6-declarative)
	- Qt SVG (qt6-svg)
	- Qt Network (qt6-base)

### Build instructions
Run the following commands:
```bash
git clone --recursive https://github.com/vimpostor/quickcurver.git
# If you forgot to clone with --recursive, just run git submodule update --init
cd quickcurver
cmake -B build
cmake --build build
```

To start QuickCurver you need to run the built executable in the `build` directory, for example on Linux run: `build/quickcurver`

## Installing compiled binaries

### Windows
Download the precompiled binary from the [latest stable release](https://github.com/vimpostor/quickcurver/releases/latest).
Extract all files and run `QuickCurver.exe` in the `release` directory.

# Multiplayer
To play multiplayer, the host starts an instance and shares the port that QuickCurver is running on. The client then just has to connect to this port on the host's ip address.
If you are not in the same local network, the host most likely has to use [Port Forwarding](https://en.wikipedia.org/wiki/Port_forwarding) to make his device available to the internet.
If a firewall is the problem, you might also want to take a look at [Hole Punching](https://en.wikipedia.org/wiki/Hole_punching_(networking)).

If network performance isn't good, the Server can tweak the "Network update rate" value in the settings, which causes data to be sent less frequently which may improve the network performance at the cost of update frequency. (A higher value means worse quality, but better network performance)

If you want to host Quickcurver cleanly on a separate server and do not need the GUI, you can start it with the CLI parameter `-platform offscreen`.
