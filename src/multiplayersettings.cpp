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

ServerSettings::ServerSettings() {
    clientSettings[0].username = "Player 0";
    clientSettings[1].username = "Bot 1";
}

QDataStream &operator <<(QDataStream &out, const ServerSettings &serverSettings) {
    out << serverSettings.playerCount;
    for (int i = 0; i < serverSettings.playerCount; ++i) {
        out << serverSettings.clientSettings[i] << serverSettings.colors[i];
    }
    return out;
}

QDataStream &operator >>(QDataStream &in, ServerSettings &serverSettings) {
    int playerCount;
    in >> playerCount;
    serverSettings.playerCount = playerCount;
    ClientSettings s;
    QColor c;
    for (int i = 0; i < playerCount; ++i) {
        in >> s >> c;
        serverSettings.clientSettings[i] = s;
        serverSettings.colors[i] = c;
    }
    return in;
}
