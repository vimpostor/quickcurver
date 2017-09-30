TEMPLATE = app

QT += gui qml quick svg quickcontrols2
CONFIG += c++14

SOURCES += src/game.cpp \
    src/main.cpp \
    src/curver.cpp \
    src/segment.cpp \
    src/gui.cpp \
    src/models/playermodel.cpp \
    src/explosion.cpp \
    src/util.cpp \
    src/headnode.cpp \
    src/items/item.cpp \
    src/items/speeditem.cpp \
    src/itemfactory.cpp \
    src/items/cleaninstallitem.cpp \
    src/models/itemmodel.cpp \
    src/wall.cpp \
    src/settings.cpp \
    src/network/client.cpp \
    src/network/network.cpp \
    src/network/server.cpp \
    src/models/chatmodel.cpp \
    src/items/invisibleitem.cpp \
    src/bot.cpp \
    src/items/agileitem.cpp \
    src/items/flashitem.cpp \
    src/items/slowitem.cpp

RESOURCES += src/qml/qml.qrc

INCLUDEPATH += src

include(fluid/fluid.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += src/game.h \
    src/curver.h \
    src/segment.h \
    src/gui.h \
    src/models/playermodel.h \
    src/explosion.h \
    src/util.h \
    src/headnode.h \
    src/items/item.h \
    src/items/speeditem.h \
    src/itemfactory.h \
    src/items/cleaninstallitem.h \
    src/models/itemmodel.h \
    src/wall.h \
    src/settings.h \
    src/network/client.h \
    src/network/network.h \
    src/network/server.h \
    src/models/chatmodel.h \
    src/items/invisibleitem.h \
    src/bot.h \
    src/items/agileitem.h \
    src/items/flashitem.h \
    src/items/slowitem.h
