#include "multiplayersettings.h"

void ClientSettings::reset() {
    username = "";
    ready = false;
}

QDataStream &operator <<(QDataStream &out, const ClientSettings &clientSettings) {
    out << clientSettings.username << clientSettings.ready;
    return out;
}

QDataStream &operator >>(QDataStream &in, ClientSettings &clientSettings) {
    QString username;
    bool ready;
    in >> username >> ready;
    clientSettings.username = username;
    clientSettings.ready = ready;
    return in;
}
