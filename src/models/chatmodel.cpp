#include "chatmodel.hpp"

ChatModel::ChatModel()
{
	m_roleNames[UserNameRole] = "username";
	m_roleNames[MessageRole] = "message";
}

/**
 * @brief Returns the number of rows in this model
 * @return The number of rows
 */
int ChatModel::rowCount(const QModelIndex &) const
{
	return static_cast<int>(m_data.size());
}

/**
 * @brief Returns the data at a given position
 * @param index The position
 * @param role The data to return
 * @return The data at the given \a index
 */
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

/**
 * @brief Returns the role names
 * @return Role names
 */
QHash<int, QByteArray> ChatModel::roleNames() const
{
	return m_roleNames;
}

/**
 * @brief Returns a singleton ChatModel instance
 * @return The singleton
 */
ChatModel &ChatModel::getSingleton()
{
	static ChatModel singleton;
	return singleton;
}

/**
 * @brief Adds a chat message to the model
 * @param username The author
 * @param message The chat message
 */
void ChatModel::appendMessage(QString username, QString message)
{
	beginInsertRows(index(m_data.size()-1).parent(), m_data.size(), m_data.size());
	m_data.push_back({username, message});
	endInsertRows();
	emit newMessage(username, message);
}
