#Quick Curver
This is an OpenGL accelerated Qt/C++ implementation of the famous game "Achtung die Kurve".

![alt tag](http://i.imgur.com/HTNalqO.png)
![alt tag](http://i.imgur.com/UAROtaY.png)

Please note that this project is yet in development and hasn't reached a stable version for now. This means that you could experience bugs, when playing this game.

#Installation
##Compiled binary
###Linux/X11
In case you have Qt5 properly installed, you can just download the executable in build/QuickCurver. The shared libraries needed are:
* libQt5Quick.so.5
* libQt5OpenGL.so.5
* libQt5Gui.so.5
* libQt5Qml.so.5
* libQt5Core.so.5
* libstdc++.so.6
* libm.so.6
* libgcc_s.so.1
* libc.so.6

You can find a script in sharedLibrariesTest.sh to test if you have everything installed.
In case you have not Qt5 installed, there will be a single static executable available some time.
##Compiling from source
Quick Curver uses [qml-material](https://github.com/papyros/qml-material) which must be installed first:
Navigate to a directory where you want to install qml-material and execute:
```{r, engine='bash', count_lines}
git clone git@github.com:papyros/qml-material.git //you can alternatively clone over HTTPS
mkdir build; cd build
qmake ..
make
make install # use sudo if necessary
```
Now we can proceed with the installation of Quick Curver itself:
Navigate to a new directory where you want to install Quick Curver and execute:
```{r, engine='bash', count_lines}
git clone git@github.com:Magnus2552/quickcurver.git //you can alternatively clone over HTTPS
cd build
qmake ..
make
./QuickCurver
```

If it fails to execute, make sure that it has execute permissions (chmod +x ./QuickCurver)
