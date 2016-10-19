#ifndef WALLNODE_H
#define WALLNODE_H

#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>

#define WALLTHICKNESS 2


class wallNode : public QSGGeometryNode {
public:
    wallNode(QSGNode *node, int fieldsize);
private:
	QSGGeometry *geometry;
};

#endif // WALLNODE_H
