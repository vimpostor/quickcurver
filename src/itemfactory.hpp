#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <QObject>
#include <QSGNode>
#include <QTime>

#include "items/cleaninstallitem.hpp"
#include "items/speeditem.hpp"
#include "models/itemmodel.hpp"
#include "models/playermodel.hpp"
#include "settings.hpp"
#include "util.hpp"

/**
 * @brief This class plans and manages all Item spawns
 */
class ItemFactory : public QObject {
	Q_OBJECT
public:
	explicit ItemFactory(QSGNode *parentNode, QObject *parent = nullptr);

	void resetRound();
	void update();
public slots:
	void integrateItem(bool spawned, unsigned int sequenceNumber, int which, QPointF pos, Item::AllowedUsers allowedUsers, int collectorIndex);
	void setWindow(QQuickWindow *w);
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
	/**
	 * @brief The window to render items in
	 */
	QQuickWindow *window = nullptr;
};

#endif // ITEMFACTORY_H
