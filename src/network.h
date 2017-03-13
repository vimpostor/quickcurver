#ifndef MULTIPLAYERSETTINGS_H
#define MULTIPLAYERSETTINGS_H

#include <QString>
#include <QDataStream>
#include <QColor>
#include <QtNetwork>

#define MAXPLAYERCOUNT 16

class ClientSettings {
public:
	QString username = "";
	bool ready = false;

	void reset();
};


QDataStream &operator <<(QDataStream &out, const ClientSettings &clientSettings);
QDataStream &operator >>(QDataStream &in, ClientSettings &clientSettings);

class ServerSettings {
public:
	explicit ServerSettings();
	int playerCount = 2;
	ClientSettings clientSettings[MAXPLAYERCOUNT];
	QColor colors[MAXPLAYERCOUNT]; // this should be in client settings too
};

QDataStream &operator <<(QDataStream &out, const ServerSettings &serverSettings);
QDataStream &operator >>(QDataStream &in, ServerSettings &serverSettings);

class Network {
public:
	static QHostAddress getLocalIpAddress();
};

class Gui {
public:
	void setQmlObject(QObject *qmlobject);
	void sendChatMessage(QString username, QString message);
	void editUsername(int index, QString username);
	void notifyGUI(QString msg, QString mode);
	void setPlayerStatus(int index, QString s);
private:
	QObject *qmlobject = NULL;
};

#endif // MULTIPLAYERSETTINGS_H
