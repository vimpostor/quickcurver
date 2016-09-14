TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    qcurver.cpp \
    game.cpp \
    segment.cpp \
    headnode.cpp \
    playerselector.cpp \
    dieanimation.cpp \
    curveitem.cpp \
    slowitem.cpp \
    fastitem.cpp

RESOURCES += \
    qml.qrc

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
    playerselector.h \
    dieanimation.h \
    curveitem.h \
    slowitem.h \
    fastitem.h
