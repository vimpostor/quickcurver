#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractListModel>
#include <QSGNode>

#include "items/item.h"
#include "items/cleaninstallitem.h"
#include "items/speeditem.h"
#include "items/invisibleitem.h"
#include "items/agileitem.h"
#include "items/flashitem.h"
#include "items/slowitem.h"
#include "items/ghostitem.h"

class ItemModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit ItemModel(QObject *parent = NULL);

	virtual int rowCount(const QModelIndex &) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void setProbability(const int row, const float probability);
	Q_INVOKABLE void setAllowedUsers(const int row, const int allowedUsers);

	static ItemModel &getSingleton();

	Item *makeRandomItem(QSGNode *parentNode, QPointF pos);
	Item *makePredefinedItem(QSGNode *parentNode, int which, QPointF pos, Item::AllowedUsers allowedUsers);
	struct ItemConfig {
		Item *(ItemModel::*constructor)(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
		QString name;
		QString description;
		float probability;
		Item::AllowedUsers allowedUsers;
		QString iconName;
	};
signals:
	void itemSpawned(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex);
private:
	QHash<int, QByteArray> m_roleNames;
	enum RoleNames {
		NameRole = Qt::UserRole,
		DescriptionRole,
		ProbabilityRole,
		AllowedUsersRole,
		IconNameRole
	};
	std::vector<ItemConfig> itemConfigs = {
		{&ItemModel::makeSpeedItem, "Speed", "Increases speed", 0.8, Item::ALLOW_COLLECTOR, "maps/directions_bike"},
		{&ItemModel::makeCleanInstallItem, "Cleaninstall", "Clears all segments", 0.5, Item::ALLOW_ALL, "communication/clear_all"},
		{&ItemModel::makeInvisibleItem, "Invisible", "Where are you?", 0.3, Item::ALLOW_COLLECTOR, "action/visibility_off"},
		{&ItemModel::makeAgileItem, "Agile", "Turn around faster", 0.2, Item::ALLOW_COLLECTOR, "image/rotate_90_degrees_ccw"},
		{&ItemModel::makeFlashItem, "Flash", "The fastest man alive", 0.2, Item::ALLOW_COLLECTOR, "image/flash_on"},
		{&ItemModel::makeSlowItem, "Freeze", "Decreases speed", 0.1, Item::ALLOW_OTHERS, "action/trending_down"},
		{&ItemModel::makeGhostItem, "Ghost", "Booh!", 0.2, Item::ALLOW_COLLECTOR, "social/sentiment_very_satisfied"},
	};
	unsigned int sequenceNumber = 0;

	// item constructors
	Item *makeSpeedItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
	Item *makeCleanInstallItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
	Item *makeInvisibleItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
	Item *makeAgileItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
	Item *makeFlashItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
	Item *makeSlowItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
	Item *makeGhostItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
};

float operator +(const float &a, const ItemModel::ItemConfig &b);
ItemModel::ItemConfig operator +(const ItemModel::ItemConfig &a, const ItemModel::ItemConfig &b);

#endif // ITEMMODEL_H
