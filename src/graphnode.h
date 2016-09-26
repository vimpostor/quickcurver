#ifndef GRAPHNODE_H
#define GRAPHNODE_H
#include <QtQuick/QQuickItem>
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QObject>

class GraphNode : public QSGNode
{
public:
	GraphNode();
	void addChildNode(QSGNode* node);
private:
};

#endif // GRAPHNODE_H
