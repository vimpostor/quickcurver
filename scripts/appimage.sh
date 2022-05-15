#!/usr/bin/env bash
# This script assumes that a cmake build ran already with build directory "build"

APPIMAGETOOL="/tmp/appimagetool"
APPDIR="/tmp/appdir"

cmake --build build --target linux-desktop-integration
# create an appdir
DESTDIR="$APPDIR" cmake --install build
# copy Qt libraries
mkdir -p "$APPDIR/usr/lib"
readelf -d build/quickcurver | while IFS= read -r LINE; do
	if [[ "$LINE" == *libQt6*.so* ]]; then
		LIB="$(echo "$LINE" | grep -oE 'libQt6.*.so[^]]*')"
		cp -L "/usr/lib/$LIB" "$APPDIR/usr/lib/"
	fi
done
# more hardcoded libraries
cp -L "/usr/lib/libQt6XcbQpa.so.6" "$APPDIR/usr/lib/"
mkdir -p "$APPDIR/usr/lib/qt6/plugins"
cp -rT "/usr/lib/qt6/plugins" "$APPDIR/usr/lib/qt6/plugins"

# Create AppRun
cp scripts/AppRun "$APPDIR/"

# Create desktop file
ln -sf "$APPDIR/usr/share/applications/quickcurver.desktop" "$APPDIR/"

# Create icon
ln -sf "$APPDIR/usr/share/icons/hicolor/512x512/apps/quickcurver.png" "$APPDIR/"

# download appimagetool
curl -qL 'https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-x86_64.AppImage' -o "$APPIMAGETOOL"
chmod +x "$APPIMAGETOOL"
$APPIMAGETOOL "$APPDIR"
