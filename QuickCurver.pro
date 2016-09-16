TEMPLATE = app

QT += qml quick opengl
CONFIG += c++11

SOURCES += main.cpp \
    qcurver.cpp \
    game.cpp \
    segment.cpp \
    headnode.cpp \
    dieanimation.cpp \
    curveitem.cpp \
    slowitem.cpp \
    fastitem.cpp \
    wallnode.cpp

RESOURCES += \
    qml.qrc \
    icons/icons.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    qcurver.h \
    game.h \
    segment.h \
    headnode.h \
    dieanimation.h \
    curveitem.h \
    slowitem.h \
    fastitem.h \
    wallnode.h
