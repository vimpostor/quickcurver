#ifndef HEADNODE_H
#define HEADNODE_H
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>

/**
 * @brief A node representing the head of a Curver
 */
class HeadNode : public QSGGeometryNode
{
public:
	HeadNode(QSGNode *parentNode, QSGFlatColorMaterial *material);
	~HeadNode();

	void setPosition(const QPointF newPos);
private:
	/**
	 * @brief The parent node in the scene graph
	 */
	QSGNode *parentNode;
	/**
	 * @brief The geometry used for the node
	 */
	QSGGeometry geometry = QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	/**
	 * @brief The current location
	 */
	QPointF pos;
};

#endif // HEADNODE_H
