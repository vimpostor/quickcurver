#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <vector>

/**
 * @brief A model managing all chat messages
 */
class ChatModel : public QAbstractListModel
{
	Q_OBJECT
public:
	ChatModel();

	virtual int rowCount(const QModelIndex &) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QHash<int, QByteArray> roleNames() const override;

	static ChatModel &getSingleton();
	void appendMessage(QString username, QString message);
signals:
	/**
	 * @brief Notifies that a new message has arrived
	 * @param username The username of the author
	 * @param message The message content
	 */
	void newMessage(QString username, QString message);
private:
	/**
	 * @brief A chat message
	 */
	class ChatMessage
	{
	public:
		/**
		 * @brief The author of the chat message
		 */
		QString username;
		/**
		 * @brief The actual chat message
		 */
		QString message;
		/**
		 * @brief The time that this message arrived
		 */
		QDateTime timestamp = QDateTime::currentDateTime();
	};
	/**
	 * @brief All chat messages
	 */
	std::vector<ChatMessage> m_data;
	/**
	 * @brief The role name strings for this model
	 */
	QHash<int, QByteArray> m_roleNames;
	/**
	 * @brief The role names for this model
	 */
	enum RoleNames {
		UserNameRole = Qt::UserRole,
		MessageRole,
		TimestampRole,
	};
};

#endif // CHATMODEL_H
