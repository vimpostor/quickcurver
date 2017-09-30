#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QAbstractListModel>
#include <vector>

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
private:
	struct ChatMessage
	{
		QString username, message;
	};
	std::vector<ChatMessage> m_data;
	QHash<int, QByteArray> m_roleNames;
	enum RoleNames {
		UserNameRole = Qt::UserRole,
		MessageRole
	};
};

#endif // CHATMODEL_H
