#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QSGNode>
#include <quartz/macros.hpp>
#include <vector>

#include "curver.hpp"
#include "gui.hpp"

/**
 * @brief A model containing all players
 */
class PlayerModel : public QAbstractListModel {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
public:
	QML_CPP_SINGLETON(PlayerModel)

	virtual int rowCount(const QModelIndex &) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void appendPlayer();
	Q_INVOKABLE void appendBot();
	Q_INVOKABLE void removePlayer(int row);
	void removeCurver(Curver *curver);
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
	void removeBots();
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
		PingRole,
	};
	/**
	 * @brief The role name strings
	 */
	QHash<int, QByteArray> m_roleNames {{NameRole, "name"}, {ColorRole, "color"}, {LeftKeyRole, "leftKey"}, {RightKeyRole, "rightKey"}, {RoundScoreRole, "roundScore"}, {TotalScoreRole, "totalScore"}, {ControllerRole, "controller"}, {PingRole, "ping"}};

	/**
	 * @brief The root node in the scene graph
	 */
	QSGNode *rootNode;
};
