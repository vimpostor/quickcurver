#ifndef MULTIPLAYERSETTINGS_H
#define MULTIPLAYERSETTINGS_H

#include <QString>
#include <QDataStream>

#define MAXPLAYERCOUNT 16

class ClientSettings {
public:
    QString username = "";
    bool ready = false;
};

QDataStream &operator <<(QDataStream &out, const ClientSettings &clientSettings);
QDataStream &operator >>(QDataStream &in, ClientSettings &clientSettings);

//class ServerSettings {
//public:

//};

#endif // MULTIPLAYERSETTINGS_H
