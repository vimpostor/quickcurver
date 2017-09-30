#include "chatmodel.h"

ChatModel::ChatModel()
{
	m_roleNames[UserNameRole] = "username";
	m_roleNames[MessageRole] = "message";
}

int ChatModel::rowCount(const QModelIndex &) const
{
	return static_cast<int>(m_data.size());
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
	auto &m = m_data[index.row()];
	switch (role) {
	case UserNameRole:
		return m.username;
	case MessageRole:
		return m.message;
	default:
		return "Unknown role";
	}
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
	return m_roleNames;
}

ChatModel &ChatModel::getSingleton()
{
	static ChatModel singleton;
	return singleton;
}

void ChatModel::appendMessage(QString username, QString message)
{
	beginInsertRows(index(m_data.size()-1).parent(), m_data.size(), m_data.size());
	m_data.push_back({username, message});
	endInsertRows();
}
