#include "segment.h"

/**
 * @brief Constructs a Segment with the given parent node, material and thickness
 * @param parentNode The parent node in the scene graph
 * @param material The material to use for all drawing calls
 * @param thickness The thickness of the segment
 */
Segment::Segment(QSGNode *parentNode, QSGFlatColorMaterial *material, const float thickness)
{
	this->parentNode = parentNode;
	this->thickness = thickness;

	geometry.setLineWidth(thickness);
	geometry.setDrawingMode(GL_QUAD_STRIP);
	geoNode.setGeometry(&geometry);
	geoNode.setMaterial(material);
	geometry.allocate(1);
	parentNode->appendChildNode(&geoNode);
}

Segment::~Segment()
{
	parentNode->removeChildNode(&geoNode);
}

/**
 * @brief Appends a new point to the segment
 * @param newPoint The point to append
 * @param angle The angle to append the point with. The thickness of the line spreads orthogonal relative to the angle
 */
void Segment::appendPoint(const QPointF newPoint, const float angle)
{
	const float normalAngle = angle + M_PI/2;
	const QPointF normalVector = thickness * QPointF(cos(normalAngle), sin(normalAngle));
	pos.push_back(newPoint + normalVector);
	pos.push_back(newPoint - normalVector);
	geometry.allocate(pos.size());
	QSGGeometry::Point2D *vertices = geometry.vertexDataAsPoint2D();
	for (int i = 0; i < static_cast<int>(pos.size()); ++i) {
		vertices[i].set(pos[i].x(), pos[i].y());
	}
	geoNode.markDirty(QSGNode::DirtyGeometry);

	lastPoint = newPoint;
}

/**
 * @brief Checks if this segment collides with a line from a to b
 * @param a The start point of the line
 * @param b The end point of the line
 * @return \c True, iif this segment intersects with the line a -> b
 */
bool Segment::checkForIntersection(QPointF a, QPointF b) const
{
	/* Given a line (a -- b) and (c -- d), we find an intersection as follows:
	 *
	 * First compute the equation A*x + B*y = C for both lines
	 * A = b.y - a.y
	 * B = a.x - b.x
	 * C = A*a.x + B*a.y
	 *
	 * Then for two lines 1 and 2 in this form, first check if
	 * det := A1*B2 - A2*B1 == 0 => Lines are parallel
	 * If this is the case, check if the lines intersect using a bounding rectangle
	 *
	 * Otherwise compute the location of the intersection as follows:
	 * x := (B2*C1 - B1*C2)/det
	 * y := (A1*C2 - A2*C1)/det
	 *
	 * Finally check if this intersection location is contained in both lines
	*/
	// epsilon is needed, because floating point operations are not that nice, when it comes to comparing them
	const float epsilon = 0.015625;
	const float firstA = b.y() - a.y();
	const float firstB = a.x() - b.x();
	const float firstC = firstA * a.x() + firstB * a.y();
	const float minX = std::min(a.x(), b.x()) - epsilon;
	const float maxX = std::max(a.x(), b.x()) + epsilon;
	const float minY = std::min(a.y(), b.y()) - epsilon;
	const float maxY = std::max(a.y(), b.y()) + epsilon;
	for (int i = 2; i < static_cast<int>(pos.size() - 1); ++i) {
		const QPointF c = pos[i-2];
		const QPointF d = pos[i];
		const float secondA = d.y() - c.y();
		const float secondB = c.x() - d.x();
		const float secondC = secondA * c.x() + secondB * c.y();
		const float det = firstA * secondB - secondA * firstB;
		if (static_cast<bool>(det)) {
			// not parallel
			const float x = (secondB * firstC - firstB * secondC) / det;
			const float y = (firstA * secondC - secondA * firstC) / det;
			// is the intersection location contained in both lines?
			if (minX <= x && x <= maxX && minY <= y && y <= maxY && \
					(std::min(c.x(), d.x()) - epsilon) <= x && (x <= std::max(c.x(), d.x()) + epsilon) && \
					(std::min(c.y(), d.y()) - epsilon) <= y && (y <= std::max(c.y(), d.y()) + epsilon)) {
				return true;
			}
		}
	}
	return false;
}
