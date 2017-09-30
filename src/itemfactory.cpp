#include "itemfactory.h"

#define SPAWN_WALL_THRESHOLD 20

ItemFactory::ItemFactory(QSGNode *parentNode, QObject *parent) : QObject(parent)
{
	this->parentNode = parentNode;
}

void ItemFactory::resetRound()
{
	items.clear();
	Util::for_each(usedItems, [](auto &i){ i->defuse(); });
	usedItems.clear();
	prepareNextItem();
}

void ItemFactory::update()
{
	if (QTime::currentTime() >= nextItemSpawn) {
		spawnItem();
		prepareNextItem();
	}
	checkCollisions();
}

void ItemFactory::integrateItem(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex)
{
	if (spawned) {
		// add the new spawned item
		items.emplace_back(std::unique_ptr<Item>(ItemModel::getSingleton().makePredefinedItem(parentNode, which, pos, allowedUsers)));
		items.back()->sequenceNumber = sequenceNumber;
	} else {
		auto it = Util::find_if(items, [&](auto &i){ return i->sequenceNumber == sequenceNumber; });
		if (it != items.end()) {
			if (collectorIndex != -1) {
				(*it)->trigger(PlayerModel::getSingleton().getCurvers()[collectorIndex]);
			}
			items.erase(it);
		}
	}
}

void ItemFactory::prepareNextItem()
{
	nextItemSpawn = QTime::currentTime().addMSecs(Util::randInt(Settings::getSingleton().getItemSpawnIntervalMin(), Settings::getSingleton().getItemSpawnIntervalMax()));
}

void ItemFactory::spawnItem()
{
	QPoint dimension = Settings::getSingleton().getDimension();
	items.emplace_back(std::unique_ptr<Item>(ItemModel::getSingleton().makeRandomItem(parentNode, QPointF(Util::randInt(SPAWN_WALL_THRESHOLD, dimension.x() - SPAWN_WALL_THRESHOLD), Util::randInt(SPAWN_WALL_THRESHOLD, dimension.y() - SPAWN_WALL_THRESHOLD)))));
}

void ItemFactory::checkCollisions()
{
	auto itemIt = items.begin();
	while (itemIt != items.end()) {
		auto curverIt = Util::find_if(PlayerModel::getSingleton().getCurvers(), [&itemIt](auto &curver){ return (*itemIt)->isInRange(curver->getPos()); });
		if (curverIt != PlayerModel::getSingleton().getCurvers().end()) {
			// trigger item
			emit ItemModel::getSingleton().itemSpawned(false, (*itemIt)->sequenceNumber, 0, QPointF(), Item::ALLOW_ALL, curverIt - PlayerModel::getSingleton().getCurvers().begin());
			(*itemIt)->trigger(*curverIt);
			usedItems.emplace_back(std::move(*itemIt));
			itemIt = items.erase(itemIt);
		} else {
			++itemIt;
		}
	}
}
