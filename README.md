#Quick Curver
This is an OpenGL accelerated Qt/C++ implementation of the famous game "Achtung die Kurve".

![alt tag](http://i.imgur.com/HTNalqO.png)
![alt tag](http://i.imgur.com/UAROtaY.png)

Please note that this project is yet in development and hasn't reached a stable version for now. This means that you could experience bugs, when playing this game.

#Installation
##Compiling from source
Quick Curver uses [qml-material](https://github.com/papyros/qml-material) which must be installed first:
Navigate to a directory where you want to install qml-material and execute:
```{r, engine='bash', count_lines}
git clone git@github.com:papyros/qml-material.git //you can alternatively clone using HTTPS
mkdir build; cd build
qmake ..
make
make install # use sudo if necessary
```
Now we can proceed with the installation of Quick Curver itself:
Navigate to a new directory where you want to install Quick Curver and execute:
```{r, engine='bash', count_lines}
git clone git@github.com:Magnus2552/quickcurver.git //you can alternatively clone using HTTPS
cd build
qmake ..
make
./QuickCurver
```

If it fails to execute, make sure that it has execute permissions (chmod +x ./QuickCurver)
