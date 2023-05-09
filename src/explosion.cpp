#include "explosion.hpp"

/**
 * @brief Creates an explosion
 * @param location The location to spawn at
 * @param parentNode The parent node in the scene graph
 * @param material The material to use for drawing calls
 * @param parent The parent object
 * @param radius The size of the explosion
 */
Explosion::Explosion(QPointF location, QSGNode *parentNode, QSGFlatColorMaterial *material, QObject *parent, float radius)
	: QObject(parent) {
	this->location = location;
	this->parentNode = parentNode;

	opacityNode = std::make_unique<QSGOpacityNode>();
	geoNode = std::make_unique<QSGGeometryNode>();
	geometry.setLineWidth(PARTICLESIZE);
	geometry.setDrawingMode(QSGGeometry::DrawLines);
	geoNode->setGeometry(&geometry);
	geoNode->setMaterial(material);

	/**
	 * We need to allocate two vertices for each line.
	 * The first point for each line is always in the origin.
	 * The second point starts at the origin, but moves outwards over time.
	 */
	geometry.allocate(2 * PARTICLECOUNT);
	vertices = geometry.vertexDataAsPoint2D();
	for (int i = 0; i < PARTICLECOUNT; ++i) {
		vertices[2 * i].set(location.x(), location.y());
		vertices[2 * i + 1].set(location.x(), location.y());
		particleDirections[i] = radius * (Util::randQPointF() - QPointF(0.5, 0.5));
	}
	opacityNode->appendChildNode(geoNode.get());
	parentNode->appendChildNode(opacityNode.get());
}

Explosion::~Explosion() {
	parentNode->removeChildNode(opacityNode.get());
}

/**
 * @brief Updates the explosion each frame
 */
void Explosion::progress() {
	float timeSinceStart = initialTime.msecsTo(QTime::currentTime());
	if (timeSinceStart > PARTICLELIFETIME) {
		// TODO: Improve lifetime of explosion, right now we only delete them at the end of each round
		return;
	} else {
		for (int i = 0; i < PARTICLECOUNT; ++i) {
			vertices[2 * i + 1].set(location.x() + PARTICLERANGE * particleDirections[i].x() * timeSinceStart / PARTICLELIFETIME,
				location.y() + PARTICLERANGE * particleDirections[i].y() * timeSinceStart / PARTICLELIFETIME);
		}
		opacityNode->setOpacity(1 - timeSinceStart / PARTICLELIFETIME);
		geoNode->markDirty(QSGNode::DirtyGeometry);
	}
}
