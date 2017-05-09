TEMPLATE = app

QT += gui qml quick svg quickcontrols2
CONFIG += c++11

SOURCES += src/items/curveitem.cpp \
	src/dieanimation.cpp \
	src/items/fastitem.cpp \
	src/game.cpp \
	src/headnode.cpp \
	src/main.cpp \
	src/qcurver.cpp \
	src/segment.cpp \
	src/wallnode.cpp \
    src/aicontroller.cpp \
    src/items/cleaninstallitem.cpp \
    src/items/fatteritem.cpp \
    src/cleaninstallanimation.cpp \
    src/network/server.cpp \
    src/network/client.cpp \
    src/network/network.cpp \
    src/items/invisibleitem.cpp


RESOURCES += src/qml/qml.qrc

include(fluid/fluid.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += src/items/curveitem.h \
	src/dieanimation.h \
	src/items/fastitem.h \
	src/game.h \
	src/headnode.h \
	src/qcurver.h \
	src/segment.h \
	src/wallnode.h \
    src/aicontroller.h \
    src/items/cleaninstallitem.h \
    src/items/fatteritem.h \
    src/cleaninstallanimation.h \
    src/network/server.h \
    src/network/client.h \
    src/network/network.h \
    src/items/invisibleitem.h
