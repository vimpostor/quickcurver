#pragma once

#include <QAbstractListModel>
#include <QSGNode>

#include "items/agileitem.hpp"
#include "items/cleaninstallitem.hpp"
#include "items/flashitem.hpp"
#include "items/ghostitem.hpp"
#include "items/invisibleitem.hpp"
#include "items/item.hpp"
#include "items/slowitem.hpp"
#include "items/speeditem.hpp"

/**
 * @brief A model containing all Item configurations
 *
 * This model does not hold spawned items. See ItemFactory for this.
 * This model stores the settings of all possible items.
 */
class ItemModel : public QAbstractListModel {
	Q_OBJECT
public:
	explicit ItemModel(QObject *parent = NULL);

	virtual int rowCount(const QModelIndex &) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void setProbability(const int row, const float probability);
	Q_INVOKABLE void setAllowedUsers(const int row, const int allowedUsers);

	static ItemModel &getSingleton();

	Item *makeRandomItem(QSGNode *parentNode, QPointF pos, QQuickWindow *win);
	Item *makePredefinedItem(QSGNode *parentNode, int which, QPointF pos, Item::AllowedUsers allowedUsers, QQuickWindow *win);
	/**
	 * @brief This struct contains all configuration options for a Item
	 */
	struct ItemConfig {
		/**
		 * @brief The constructing function for the Item in question
		 */
		Item *(ItemModel::*constructor)(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
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
		{&ItemModel::makeSpeedItem, "Speed", "Increases speed", 0.8, Item::AllowedUsers::ALLOW_COLLECTOR, "fast_forward"},
		{&ItemModel::makeCleanInstallItem, "Cleaninstall", "Clears all segments", 0.5, Item::AllowedUsers::ALLOW_ALL, "clear_all"},
		{&ItemModel::makeInvisibleItem, "Invisible", "Where are you?", 0.3, Item::AllowedUsers::ALLOW_COLLECTOR, "mystery"},
		{&ItemModel::makeAgileItem, "Agile", "Turn around faster", 0.2, Item::AllowedUsers::ALLOW_COLLECTOR, "rotate_90_degrees_ccw"},
		{&ItemModel::makeFlashItem, "Flash", "The fastest man alive", 0.2, Item::AllowedUsers::ALLOW_COLLECTOR, "flash_on"},
		{&ItemModel::makeSlowItem, "Freeze", "Decreases speed", 0.1, Item::AllowedUsers::ALLOW_OTHERS, "fast_rewind"},
		{&ItemModel::makeGhostItem, "Ghost", "Booh!", 0.0, Item::AllowedUsers::ALLOW_OTHERS, "mystery"},
	};
	/**
	 * @brief The sequence number of the last spawned Item
	 */
	unsigned int sequenceNumber = 0;

	// item constructors
	Item *makeSpeedItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
	Item *makeCleanInstallItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
	Item *makeInvisibleItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
	Item *makeAgileItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
	Item *makeFlashItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
	Item *makeSlowItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
	Item *makeGhostItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos, QQuickWindow *win);
};

float operator+(const float &a, const ItemModel::ItemConfig &b);
ItemModel::ItemConfig operator+(const ItemModel::ItemConfig &a, const ItemModel::ItemConfig &b);
