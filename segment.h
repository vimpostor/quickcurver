#ifndef SEGMENT_H
#define SEGMENT_H
#include <QtQuick/QQuickItem>
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QSGGeometry>
#include <QMutex>
#include <QTime>
#define MAXPOSSIZE 600


class segment : public QObject
{
	Q_OBJECT
public:
	segment(QColor color, QSGNode *node, QSGFlatColorMaterial* material);
	~segment();
	QPointF pos[MAXPOSSIZE];
	short int poscount = 0;
	void appendPoint(QPointF addedPoint);
	QPointF getLastPoint();
	bool checkforIntersection(QPointF a, QPointF b);
	static int randInt(int min, int max);
	static float randFloat();
	static void initRand();
signals:

public slots:

private:
	QColor color;
	QSGGeometryNode* gnode;
	QSGGeometry* geometry;
	QMutex nodeMutex;
	int thickness = 16;
	QSGNode* node;
};

#endif // SEGMENT_H
