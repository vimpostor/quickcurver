#include "explosion.h"

/**
 * @brief Creates an explosion
 * @param location The location to spawn at
 * @param parentNode The parent node in the scene graph
 * @param material The material to use for drawing calls
 * @param parent The parent object
 */
Explosion::Explosion(QPointF location, QSGNode *parentNode, QSGFlatColorMaterial *material, QObject *parent, float radius) : QObject(parent)
{
	this->location = location;
	this->parentNode = parentNode;

	opacityNode = std::make_unique<QSGOpacityNode>();
	geoNode = std::make_unique<QSGGeometryNode>();
	geometry.setLineWidth(PARTICLESIZE);
	geometry.setDrawingMode(QSGGeometry::DrawPoints);
	geoNode->setGeometry(&geometry);
	geoNode->setMaterial(material);
	geometry.allocate(PARTICLECOUNT);
	vertices = geometry.vertexDataAsPoint2D();
	for (int i = 0; i < PARTICLECOUNT; ++i) {
		vertices[i].set(location.x(), location.y());
		particleDirections[i] = radius * (Util::randQPointF() - QPointF(0.5, 0.5));
	}
	opacityNode->appendChildNode(geoNode.get());
	parentNode->appendChildNode(opacityNode.get());
	connect(&timer, &QTimer::timeout, this, &Explosion::progress);
	timer.start(16);
}

Explosion::~Explosion()
{
	parentNode->removeChildNode(opacityNode.get());
}

/**
 * @brief Updates the explosion each frame
 */
void Explosion::progress()
{
	float timeSinceStart = initialTime.msecsTo(QTime::currentTime());
	if (timeSinceStart > PARTICLELIFETIME) {
		delete this;
	} else {
		for (int i = 0; i < PARTICLECOUNT; ++i) {
			vertices[i].set(location.x() + PARTICLERANGE * particleDirections[i].x() * timeSinceStart / PARTICLELIFETIME, \
							location.y() + PARTICLERANGE * particleDirections[i].y() * timeSinceStart / PARTICLELIFETIME);
		}
		opacityNode->setOpacity(1 - timeSinceStart / PARTICLELIFETIME);
		geoNode->markDirty(QSGNode::DirtyGeometry);
	}
}
