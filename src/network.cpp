#include "network.h"


// ClientSettings

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


// ServerSettings

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


// Network

QHostAddress Network::getLocalIpAddress() {
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
			return ipAddressesList.at(i);
		}
	}
	// if we did not find one, use IPv4 localhost
	return QHostAddress(QHostAddress::LocalHost);
}


// Gui

void Gui::setQmlObject(QObject *qmlobject) {
	this->qmlobject = qmlobject;
}

void Gui::sendChatMessage(QString username, QString message) {
	QVariant returnedValue;
	QMetaObject::invokeMethod(qmlobject, "sendMessage", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, username), Q_ARG(QVariant, message));
}

void Gui::editUsername(int index, QString username) {
	QVariant returnedValue;
	QMetaObject::invokeMethod(qmlobject, "clientEditPlayer", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, index), Q_ARG(QVariant, username));
}

void Gui::notifyGUI(QString msg, QString mode) {
	QVariant returnedValue;
	QMetaObject::invokeMethod(qmlobject, "notifyGUI", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg), Q_ARG(QVariant, mode));
}

void Gui::setPlayerStatus(int index, QString s) {
	QVariant returnedValue;
	QMetaObject::invokeMethod(qmlobject, "setPlayerStatus", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, index), Q_ARG(QVariant, s));
}
