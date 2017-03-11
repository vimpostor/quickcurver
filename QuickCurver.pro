TEMPLATE = app

QT += qml quick svg
CONFIG += c++11

SOURCES += src/curveitem.cpp \
	src/dieanimation.cpp \
	src/fastitem.cpp \
	src/game.cpp \
	src/headnode.cpp \
	src/main.cpp \
	src/qcurver.cpp \
	src/segment.cpp \
	src/wallnode.cpp \
    src/aicontroller.cpp \
    src/cleaninstallitem.cpp \
    src/fatteritem.cpp \
    src/cleaninstallanimation.cpp \
    src/server.cpp \
    src/client.cpp \
    src/multiplayersettings.cpp \
    src/invisibleitem.cpp


RESOURCES += \
    src/qml/qml.qrc \
    icons/icons.qrc


# qml-material
DEFINES += QPM_INIT\\(E\\)=\"E.addImportPath(QStringLiteral(\\\"qrc:/\\\"));\"
include(material/material.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += src/curveitem.h \
	src/dieanimation.h \
	src/fastitem.h \
	src/game.h \
	src/headnode.h \
	src/qcurver.h \
	src/segment.h \
	src/wallnode.h \
    src/aicontroller.h \
    src/cleaninstallitem.h \
    src/fatteritem.h \
    src/cleaninstallanimation.h \
    src/server.h \
    src/client.h \
    src/multiplayersettings.h \
    src/invisibleitem.h
