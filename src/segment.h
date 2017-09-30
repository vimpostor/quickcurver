#ifndef SEGMENT_H
#define SEGMENT_H

#include <QQuickItem>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <memory>
#include <QtMath>

class Segment : public QObject
{
	Q_OBJECT
public:
	explicit Segment(QSGNode *parentNode, QSGFlatColorMaterial *material, const float thickness);
	~Segment();

	void appendPoint(const QPointF newPoint, const float angle);
	bool checkForIntersection(QPointF a, QPointF b) const;
private:
	QSGNode *parentNode;
	float thickness;
	QSGGeometryNode geoNode;
	QSGGeometry geometry = QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	std::vector<QPointF> pos;
	QPointF lastPoint;
};

#endif // SEGMENT_H
