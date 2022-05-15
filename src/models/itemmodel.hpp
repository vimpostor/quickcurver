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

/**
 * @brief A model containing all Item configurations
 *
 * This model does not hold spawned items. See ItemFactory for this.
 * This model stores the settings of all possible items.
 */
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
	/**
	 * @brief This struct contains all configuration options for a Item
	 */
	struct ItemConfig {
		/**
		 * @brief The constructing function for the Item in question
		 */
		Item *(ItemModel::*constructor)(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos);
		/**
		 * @brief The name of the Item
		 */
		QString name;
		/**
		 * @brief A short description of the Item
		 */
		QString description;
		/**
		 * @brief The spawn probability of the Item
		 *
		 * Must be between 0 and 1
		 */
		float probability;
		/**
		 * @brief The allowed users for this Item
		 */
		Item::AllowedUsers allowedUsers;
		/**
		 * @brief The icon name for this Item
		 */
		QString iconName;
	};
signals:
	/**
	 * @brief Emitted when a new Item event happened
	 * @param spawned Whether the Item spawned or was triggered
	 * @param sequenceNumber The unique sequence number of the Item
	 * @param which The kind of the Item
	 * @param pos The location of the Item
	 * @param allowedUsers The allowed users for the Item
	 * @param collectorIndex If \a spawned is \c false, this value indicates the collecting Curver
	 */
	void itemSpawned(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex);
private:
	/**
	 * @brief The role name strings for this model
	 */
	QHash<int, QByteArray> m_roleNames;
	/**
	 * @brief The role names for this model
	 */
	enum RoleNames {
		NameRole = Qt::UserRole,
		DescriptionRole,
		ProbabilityRole,
		AllowedUsersRole,
		IconNameRole
	};
	/**
	 * @brief A vector containing all item configurations
	 */
	std::vector<ItemConfig> itemConfigs = {
		{&ItemModel::makeSpeedItem, "Speed", "Increases speed", 0.8, Item::AllowedUsers::ALLOW_COLLECTOR, "maps/directions_bike"},
		{&ItemModel::makeCleanInstallItem, "Cleaninstall", "Clears all segments", 0.5, Item::AllowedUsers::ALLOW_ALL, "communication/clear_all"},
		{&ItemModel::makeInvisibleItem, "Invisible", "Where are you?", 0.3, Item::AllowedUsers::ALLOW_COLLECTOR, "action/visibility_off"},
		{&ItemModel::makeAgileItem, "Agile", "Turn around faster", 0.2, Item::AllowedUsers::ALLOW_COLLECTOR, "image/rotate_90_degrees_ccw"},
		{&ItemModel::makeFlashItem, "Flash", "The fastest man alive", 0.2, Item::AllowedUsers::ALLOW_COLLECTOR, "image/flash_on"},
		{&ItemModel::makeSlowItem, "Freeze", "Decreases speed", 0.1, Item::AllowedUsers::ALLOW_OTHERS, "action/trending_down"},
		{&ItemModel::makeGhostItem, "Ghost", "Booh!", 0.0, Item::AllowedUsers::ALLOW_OTHERS, "social/sentiment_very_satisfied"},
	};
	/**
	 * @brief The sequence number of the last spawned Item
	 */
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
