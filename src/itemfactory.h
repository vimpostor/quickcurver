#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <QObject>
#include <QTime>
#include <QSGNode>

#include "models/playermodel.h"
#include "util.h"
#include "items/speeditem.h"
#include "items/cleaninstallitem.h"
#include "models/itemmodel.h"
#include "settings.h"

/**
 * @brief This class plans and manages all Item spawns
 */
class ItemFactory : public QObject
{
	Q_OBJECT
public:
	explicit ItemFactory(QSGNode *parentNode, QObject *parent = nullptr);

	void resetRound();
	void update();
public slots:
	void integrateItem(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex);
signals:
private:
	void prepareNextItem();
	void spawnItem();
	void checkCollisions();

	/**
	 * @brief The parent node in the scene graph
	 */
	QSGNode *parentNode;
	/**
	 * @brief The time of the next Item spawn
	 */
	QTime nextItemSpawn;
	/**
	 * @brief All currently available visible Item instances
	 */
	std::vector<std::unique_ptr<Item>> items;
	/**
	 * @brief All used Item instances waiting to be deleted
	 */
	std::vector<std::unique_ptr<Item>> usedItems;
};

#endif // ITEMFACTORY_H
