#ifndef CLEANINSTALLANIMATION_H
#define CLEANINSTALLANIMATION_H
#include <QPointF>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGOpacityNode>
#include <QTime>
#include <QTimer>
#include "segment.h"
#include <QObject>

#define ANIMATIONDURATION 200
#define MAXPOSITIONSIZE 4048


class CleanInstallAnimation : public QObject
{
	Q_OBJECT
public:
	CleanInstallAnimation(QSGNode *node, QSGMaterial *material, QObject *parent = 0);
    ~CleanInstallAnimation();
	void addSegment(segment *s);
	void fireAnimation();
signals:
private:
	QSGNode *node;
	QSGOpacityNode *onode;
	QSGGeometryNode *gnode;
	QSGGeometry *geometry;
	QTime lastTime, initialTime;
	QTimer *timer;
	void progress(float timeSinceStart);
	QPointF pos[MAXPOSITIONSIZE];
	int posCount = 0;
private slots:
	void progress();
};

#endif // CLEANINSTALLANIMATION_H
