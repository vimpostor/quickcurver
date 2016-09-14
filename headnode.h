#ifndef HEADNODE_H
#define HEADNODE_H
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>


class headNode : public QSGGeometryNode
{
public:
	headNode(QPointF startPos, QSGFlatColorMaterial* material, QSGNode* node);
	void updatePosition(QPointF newPos);
private:
	QPointF pos;
	QSGGeometry* geometry;
	short int thickness = 6;
};

#endif // HEADNODE_H
