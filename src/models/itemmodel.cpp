#include "itemmodel.h"

/**
 * @brief Constructs an ItemModel
 * @param parent The parent object
 */
ItemModel::ItemModel(QObject *parent) : QAbstractListModel(parent)
{
	// role names
	m_roleNames[NameRole] = "name";
	m_roleNames[DescriptionRole] = "description";
	m_roleNames[ProbabilityRole] = "probability";
	m_roleNames[AllowedUsersRole] = "allowedUsers";
	m_roleNames[IconNameRole] = "iconName";
}

/**
 * @brief Returns the number of rows stored in this model
 * @return The row count
 */
int ItemModel::rowCount(const QModelIndex &) const
{
	return itemConfigs.size();
}

/**
 * @brief Returns specific data stored in this model
 * @param index The index of the data
 * @param role The role to access
 * @return The data
 */
QVariant ItemModel::data(const QModelIndex &index, int role) const
{
	const auto &itemConfig = itemConfigs[index.row()];
	switch (role) {
		case NameRole:
			return itemConfig.name;
		case DescriptionRole:
			return itemConfig.description;
		case ProbabilityRole:
			return itemConfig.probability;
		case AllowedUsersRole:
			return static_cast<int>(itemConfig.allowedUsers);
		case IconNameRole:
			return itemConfig.iconName;
		default:
			return "Unkown role";
	}
}

/**
 * @brief Returns the role names of this model
 * @return The role names
 */
QHash<int, QByteArray> ItemModel::roleNames() const
{
	return m_roleNames;
}

/**
 * @brief Sets the probability of an Item
 * @param row The index of the Item
 * @param probability The new probability
 */
void ItemModel::setProbability(const int row, const float probability)
{
	itemConfigs[static_cast<unsigned long>(row)].probability = probability;
}

/**
 * @brief Sets the allowed users for an Item
 * @param row The index of the Item
 * @param allowedUsers The allowed users for this Item
 */
void ItemModel::setAllowedUsers(const int row, const int allowedUsers)
{
	itemConfigs[static_cast<unsigned long>(row)].allowedUsers = static_cast<Item::AllowedUsers>(allowedUsers);
}

/**
 * @brief Returns a singleton instance of ItemModel
 * @return The singleton
 */
ItemModel &ItemModel::getSingleton()
{
	static ItemModel singleton;
	return singleton;
}

/**
 * @brief Creates a random Item at a given position
 * @param parentNode The parent node in the scene graph
 * @param pos THe location of the Item
 * @return The just created Item
 */
Item *ItemModel::makeRandomItem(QSGNode *parentNode, QPointF pos)
{
	float totalProbability = Util::accumulate(itemConfigs, 0.f);
	std::vector<ItemConfig> partialSums = itemConfigs;
	Util::partial_sum(itemConfigs, partialSums);
	float randValue = Util::rand() * totalProbability;
	auto it = Util::find_if(partialSums, [randValue](auto &item){ return randValue < item.probability; });
	auto *result = (this->*it->constructor)(parentNode, Util::expandIconName(it->iconName), it->allowedUsers, pos);
	result->sequenceNumber = ++sequenceNumber;
	emit itemSpawned(true, result->sequenceNumber, it - partialSums.begin(), pos, it->allowedUsers, -1);
	return result;
}

/**
 * @brief Creates a predetermined Item
 * @param parentNode The parent node in the scene graph
 * @param which The kind of Item
 * @param pos The location of the Item
 * @param allowedUsers The allowed users for this Item
 * @return The just created Item
 */
Item *ItemModel::makePredefinedItem(QSGNode *parentNode, int which, QPointF pos, Item::AllowedUsers allowedUsers)
{
	auto conf = itemConfigs[which];
	return (this->*conf.constructor)(parentNode, Util::expandIconName(conf.iconName), allowedUsers, pos);
}

/**
 * @brief Constructs a SpeedItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The path to the icon
 * @param allowedUsers The allowed users
 * @param pos The location
 * @return The just created item
 */
Item *ItemModel::makeSpeedItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new SpeedItem(parentNode, iconName, allowedUsers, pos);
}

/**
 * @brief Constructs a CleanInstallItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The path to the icon
 * @param allowedUsers The allowed users
 * @param pos The location
 * @return The just created item
 */
Item *ItemModel::makeCleanInstallItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new CleanInstallItem(parentNode, iconName, allowedUsers, pos);
}

/**
 * @brief Constructs an InvisibleItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The path to the icon
 * @param allowedUsers The allowed users
 * @param pos The location
 * @return The just created item
 */
Item *ItemModel::makeInvisibleItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new InvisibleItem(parentNode, iconName, allowedUsers, pos);
}

/**
 * @brief Constructs an AgileItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The path to the icon
 * @param allowedUsers The allowed users
 * @param pos The location
 * @return The just created item
 */
Item *ItemModel::makeAgileItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new AgileItem(parentNode, iconName, allowedUsers, pos);
}

/**
 * @brief Constructs a FlashItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The path to the icon
 * @param allowedUsers The allowed users
 * @param pos The location
 * @return The just created item
 */
Item *ItemModel::makeFlashItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new FlashItem(parentNode, iconName, allowedUsers, pos);
}

/**
 * @brief Constructs a SlowItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The path to the icon
 * @param allowedUsers The allowed users
 * @param pos The location
 * @return The just created item
 */
Item *ItemModel::makeSlowItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new SlowItem(parentNode, iconName, allowedUsers, pos);
}

/**
 * @brief Constructs a GhostItem
 * @param parentNode The parent node in the scene graph
 * @param iconName The path to the icon
 * @param allowedUsers The allowed users
 * @param pos The location
 * @return The just created item
 */
Item *ItemModel::makeGhostItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new GhostItem(parentNode, iconName, allowedUsers, pos);
}

/**
 * @brief Adds two probabilities
 * @param a A given initial probability
 * @param b The ItemModel::ItemConfig containing a probability
 * @return The accumulated probability
 */
float operator +(const float &a, const ItemModel::ItemConfig &b)
{
	return a + b.probability;
}

/**
 * @brief Adds two probabilities
 * @param a A configuration with a probability
 * @param b Another configuration with a probability
 * @return The accumulated probability
 */
ItemModel::ItemConfig operator +(const ItemModel::ItemConfig &a, const ItemModel::ItemConfig &b)
{
	ItemModel::ItemConfig result = b;
	result.probability = a.probability + b;
	return result;
}
