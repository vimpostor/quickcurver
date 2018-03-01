#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QAbstractListModel>
#include <vector>
#include <QHash>
#include <QSGNode>

#include "curver.h"
#include "gui.h"

/**
 * @brief A model containing all players
 */
class PlayerModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit PlayerModel(QObject *parent = NULL);

	virtual int rowCount(const QModelIndex &) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QHash<int, QByteArray> roleNames() const override;

	static PlayerModel &getSingleton();

	Q_INVOKABLE void appendPlayer();
	Q_INVOKABLE void removePlayer(int row);
	Q_INVOKABLE void setColor(int row, QColor color);
	Q_INVOKABLE void setLeftKey(int row, Qt::Key key);
	Q_INVOKABLE void setRightKey(int row, Qt::Key key);
	Q_INVOKABLE void setUserName(int row, QString username);
	Q_INVOKABLE void setController(int row, int ctrl);

	void setRootNode(QSGNode *rootNode);
	std::vector<std::unique_ptr<Curver>> &getCurvers();
	void serialize(QDataStream &out) const;
	void parse(QDataStream &in);
	Curver *getNewPlayer();
	void forceRefresh();
public slots:
	void processDeath();
signals:
	/**
	 * @brief Emitted when a Curver died
	 */
	void curverDied();
	/**
	 * @brief Emitted when the model changed
	 */
	void playerModelChanged();
private:
	/**
	 * @brief The model data
	 */
	std::vector<std::unique_ptr<Curver>> m_data;
	/**
	 * @brief The role name strings
	 */
	QHash<int, QByteArray> m_roleNames;
	/**
	 * @brief The role names
	 */
	enum RoleNames {
		NameRole = Qt::UserRole,
		ColorRole,
		LeftKeyRole,
		RightKeyRole,
		RoundScoreRole,
		TotalScoreRole,
		ControllerRole,
	};
	/**
	 * @brief The root node in the scene graph
	 */
	QSGNode *rootNode;
};

#endif // PLAYERMODEL_H
