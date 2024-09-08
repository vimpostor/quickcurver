#include "itemfactory.hpp"

#define SPAWN_WALL_THRESHOLD 20

/**
 * @brief Constructs a ItemFactory
 * @param parentNode The parent node in the scene graph
 * @param parent The parent object
 */
ItemFactory::ItemFactory(QSGNode *parentNode, QObject *parent)
	: QObject(parent) {
	this->parentNode = parentNode;
}

/**
 * @brief Resets the round causing all Item instances to disappear
 */
void ItemFactory::resetRound() {
	items.clear();
	std::ranges::for_each(usedItems, [](auto &i) { i->defuse(); });
	usedItems.clear();
	prepareNextItem();
}

/**
 * @brief Updates the ItemFactory to check whether a new Item should spawn
 *
 * Also checks if any Curver triggers an Item
 */
void ItemFactory::update() {
	if (nextItemSpawn.isValid() && QTime::currentTime() >= nextItemSpawn) {
		spawnItem();
		prepareNextItem();
	}
	checkCollisions();

	// update all items
	std::ranges::for_each(items, [](auto &i) { i->update(); });
	std::ranges::for_each(usedItems, [](auto &i) { i->update(); });
}

/**
 * @brief Integrates a predetermined Item event into the ItemFactory
 * @param spawned Whether the Item spawned or was triggered
 * @param sequenceNumber The unique sequence number of the Item
 * @param which The kind of Item
 * @param pos The location of the Item
 * @param allowedUsers The allowed users of the Item
 * @param collectorIndex If \a spawned is \c false, this determines the collecting Curver
 */
void ItemFactory::integrateItem(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex) {
	if (spawned) {
		// add the new spawned item
		items.emplace_back(std::unique_ptr<Item>(ItemModel::get()->makePredefinedItem(parentNode, which, pos, allowedUsers, window)));
		items.back()->sequenceNumber = sequenceNumber;
	} else {
		auto it = std::ranges::find_if(items, [&](auto &i) { return i->sequenceNumber == sequenceNumber; });
		if (it != items.end()) {
			if (collectorIndex != -1) {
				(*it)->trigger(PlayerModel::get()->getCurvers()[collectorIndex]);
			}
			items.erase(it);
		}
	}
}

/**
 * @brief Sets the new window
 * @param w The new window
 */
void ItemFactory::setWindow(QQuickWindow *w) {
	window = w;
}

/**
 * @brief Prepares a new Item spawn
 */
void ItemFactory::prepareNextItem() {
	nextItemSpawn = QTime::currentTime().addMSecs(Util::randInt(Settings::get()->getItemSpawnIntervalMin(), Settings::get()->getItemSpawnIntervalMax()));
}

/**
 * @brief Spawns a new Item
 */
void ItemFactory::spawnItem() {
	QPoint dimension = Settings::get()->getDimension();
	items.emplace_back(std::unique_ptr<Item>(ItemModel::get()->makeRandomItem(parentNode, QPointF(Util::randInt(SPAWN_WALL_THRESHOLD, dimension.x() - SPAWN_WALL_THRESHOLD), Util::randInt(SPAWN_WALL_THRESHOLD, dimension.y() - SPAWN_WALL_THRESHOLD)), window)));
}

/**
 * @brief Checks if any Curver is in range of any Item and triggers it accordingly
 */
void ItemFactory::checkCollisions() {
	auto itemIt = items.begin();
	while (itemIt != items.end()) {
		auto curverIt = std::ranges::find_if(PlayerModel::get()->getCurvers(), [&itemIt](auto &curver) { return (*itemIt)->isInRange(curver->getPos()); });
		if (curverIt != PlayerModel::get()->getCurvers().end()) {
			// trigger item
			ItemModel::get()->itemSpawned(false, (*itemIt)->sequenceNumber, 0, QPointF(), Item::AllowedUsers::ALLOW_ALL, curverIt - PlayerModel::get()->getCurvers().begin());
			(*itemIt)->trigger(*curverIt);
			usedItems.emplace_back(std::move(*itemIt));
			itemIt = items.erase(itemIt);
		} else {
			++itemIt;
		}
	}
}
