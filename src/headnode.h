#ifndef HEADNODE_H
#define HEADNODE_H
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>

class HeadNode : public QSGGeometryNode
{
public:
	HeadNode(QSGNode *parentNode, QSGFlatColorMaterial *material);
	~HeadNode();

	void setPosition(const QPointF newPos);
private:
	QSGNode *parentNode;
	QSGGeometry geometry = QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	QPointF pos;
};

#endif // HEADNODE_H
