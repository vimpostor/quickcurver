#include "itemmodel.h"

ItemModel::ItemModel(QObject *parent) : QAbstractListModel(parent)
{
	// role names
	m_roleNames[NameRole] = "name";
	m_roleNames[DescriptionRole] = "description";
	m_roleNames[ProbabilityRole] = "probability";
	m_roleNames[AllowedUsersRole] = "allowedUsers";
	m_roleNames[IconNameRole] = "iconName";
}

int ItemModel::rowCount(const QModelIndex &) const
{
	return itemConfigs.size();
}

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

QHash<int, QByteArray> ItemModel::roleNames() const
{
	return m_roleNames;
}

void ItemModel::setProbability(const int row, const float probability)
{
	itemConfigs[static_cast<unsigned long>(row)].probability = probability;
}

void ItemModel::setAllowedUsers(const int row, const int allowedUsers)
{
	itemConfigs[static_cast<unsigned long>(row)].allowedUsers = static_cast<Item::AllowedUsers>(allowedUsers);
}

ItemModel &ItemModel::getSingleton()
{
	static ItemModel singleton;
	return singleton;
}

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

Item *ItemModel::makePredefinedItem(QSGNode *parentNode, int which, QPointF pos, Item::AllowedUsers allowedUsers)
{
	auto conf = itemConfigs[which];
	return (this->*conf.constructor)(parentNode, Util::expandIconName(conf.iconName), allowedUsers, pos);
}

Item *ItemModel::makeSpeedItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new SpeedItem(parentNode, iconName, allowedUsers, pos);
}

Item *ItemModel::makeCleanInstallItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new CleanInstallItem(parentNode, iconName, allowedUsers, pos);
}

Item *ItemModel::makeInvisibleItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new InvisibleItem(parentNode, iconName, allowedUsers, pos);
}

Item *ItemModel::makeAgileItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new AgileItem(parentNode, iconName, allowedUsers, pos);
}

Item *ItemModel::makeFlashItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new FlashItem(parentNode, iconName, allowedUsers, pos);
}

Item *ItemModel::makeSlowItem(QSGNode *parentNode, QString iconName, Item::AllowedUsers allowedUsers, QPointF pos)
{
	return new SlowItem(parentNode, iconName, allowedUsers, pos);
}

float operator +(const float &a, const ItemModel::ItemConfig &b)
{
	return a + b.probability;
}

ItemModel::ItemConfig operator +(const ItemModel::ItemConfig &a, const ItemModel::ItemConfig &b)
{
	ItemModel::ItemConfig result = b;
	result.probability = a.probability + b;
	return result;
}
