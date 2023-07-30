#pragma once

#include <QQuickItem>
#include <QSGFlatColorMaterial>
#include <QSGNode>
#include <QtMath>
#include <memory>
#include <optional>

/**
 * @brief A class representing a segment of a line
 *
 * Every Curver consists of multiple Segment lines. This class represents a single instance of such a line.
 */
class Segment : public QObject {
	Q_OBJECT
public:
	explicit Segment(QSGNode *parentNode, QSGFlatColorMaterial *material, const float thickness);
	~Segment();

	void appendPoint(const QPointF newPoint, const float angle);
	bool checkForIntersection(QPointF a, QPointF b) const;
	size_t getSegmentSize() const;
	void popPoints(const size_t amount);
	void clear();
	std::optional<QPointF> getFirstPos() const;
private:
	void updateGeometry();

	/**
	 * @brief The parent node in the scene graph
	 */
	QSGNode *parentNode;
	/**
	 * @brief The thickness of the line
	 */
	float thickness;
	/**
	 * @brief The node representing this Segment in the scene graph
	 */
	QSGGeometryNode geoNode;
	/**
	 * @brief The geometry of this Segment.
	 */
	QSGGeometry geometry = QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	/**
	 * @brief Every position that has to be stored in Segment::geometry.
	 */
	std::vector<QPointF> pos;
	/**
	 * @brief The last point that was added to this Segment.
	 */
	QPointF lastPoint;
};
