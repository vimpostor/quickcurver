TEMPLATE = app

QT += gui qml quick svg quickcontrols2
CONFIG += c++14

RESOURCES += qml/qml.qrc ../fluid/src/imports/controls/icons.qrc

QML_IMPORT_PATH = $$OUT_PWD/../fluid/qml

DEFINES += QT_DEPRECATED_WARNINGS

android {
	# Bundle Fluid QML plugins with the application
	ANDROID_EXTRA_PLUGINS = $$OUT_PWD/../fluid/qml

	# Android package sources
	ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

macx {
	# Bundle Fluid QML plugins with the application
	APP_QML_FILES.files = $$OUT_PWD/../fluid/qml/Fluid
	APP_QML_FILES.path = Contents/MacOS
	QMAKE_BUNDLE_DATA += APP_QML_FILES
}

win32 {
	WINDEPLOYQT_OPTIONS = -qmldir $$OUT_PWD/../fluid/qml/Fluid
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = QuickCurver

SOURCES += game.cpp \
    main.cpp \
    curver.cpp \
    segment.cpp \
    gui.cpp \
    models/playermodel.cpp \
    explosion.cpp \
    util.cpp \
    headnode.cpp \
    items/item.cpp \
    items/speeditem.cpp \
    itemfactory.cpp \
    items/cleaninstallitem.cpp \
    models/itemmodel.cpp \
    wall.cpp \
    settings.cpp \
    network/client.cpp \
    network/network.cpp \
    network/server.cpp \
    models/chatmodel.cpp \
    items/invisibleitem.cpp \
    bot.cpp \
    items/agileitem.cpp \
    items/flashitem.cpp \
    items/slowitem.cpp \
    items/ghostitem.cpp

HEADERS += game.h \
    curver.h \
    segment.h \
    gui.h \
    models/playermodel.h \
    explosion.h \
    util.h \
    headnode.h \
    items/item.h \
    items/speeditem.h \
    itemfactory.h \
    items/cleaninstallitem.h \
    models/itemmodel.h \
    wall.h \
    settings.h \
    network/client.h \
    network/network.h \
    network/server.h \
    models/chatmodel.h \
    items/invisibleitem.h \
    bot.h \
    items/agileitem.h \
    items/flashitem.h \
    items/slowitem.h \
    items/ghostitem.h
