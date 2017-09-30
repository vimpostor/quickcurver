#include "explosion.h"

Explosion::Explosion(QPointF location, QSGNode *parentNode, QSGFlatColorMaterial *material, QObject *parent) : QObject(parent)
{
	this->location = location;
	this->parentNode = parentNode;

	opacityNode = std::make_unique<QSGOpacityNode>();
	geoNode = std::make_unique<QSGGeometryNode>();
	geometry.setLineWidth(PARTICLESIZE);
	geometry.setDrawingMode(GL_POINTS);
	geoNode->setGeometry(&geometry);
	geoNode->setMaterial(material);
	geometry.allocate(PARTICLECOUNT);
	vertices = geometry.vertexDataAsPoint2D();
	for (int i = 0; i < PARTICLECOUNT; ++i) {
		vertices[i].set(location.x(), location.y());
		particleDirections[i] = Util::randQPointF() - QPointF(0.5, 0.5);
	}
	opacityNode->appendChildNode(geoNode.get());
	parentNode->appendChildNode(opacityNode.get());
	connect(&timer, SIGNAL(timeout()), this, SLOT(progress()));
	timer.start(16);
}

Explosion::~Explosion()
{
	parentNode->removeChildNode(opacityNode.get());
}

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
