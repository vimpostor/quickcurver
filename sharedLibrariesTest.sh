#!/usr/bin/env bash
for i in libQt5Quick.so.5 libQt5OpenGL.so.5 libQt5Gui.so.5 libQt5Qml.so.5 libQt5Core.so.5 libstdc++.so.6 libm.so.6 libgcc_s.so.1 libc.so.6; do
	if [[ -e /usr/lib/$i ]]; then
		: #file exists
	else
		echo "File " $i " does not exist"
		exit 1
	fi
done
echo "Everything installed!"