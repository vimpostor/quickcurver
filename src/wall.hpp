#ifndef WALL_H
#define WALL_H

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

#include "util.hpp"
#include "settings.hpp"

/**
 * @brief A node that visualizes the game borders in the scene graph
 */
class Wall : public QObject
{
	Q_OBJECT
public:
	explicit Wall();
	void setParentNode(QSGNode *parentNode);
	void updateDimension();
private:
	void resize();

	/**
	 * @brief The dimension of the game
	 */
	QPoint dimension {20, 20};
	/**
	 * @brief The node handling the geometry
	 */
	QSGGeometryNode geoNode;
	/**
	 * @brief The actual geometry
	 */
	QSGGeometry geometry = QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	/**
	 * @brief The material of the Wall
	 */
	QSGFlatColorMaterial material;
};

#endif // WALL_H
