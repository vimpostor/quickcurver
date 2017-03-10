#include "cleaninstallanimation.h"

CleanInstallAnimation::CleanInstallAnimation(QSGNode *node, QSGMaterial *material, QObject *parent) : QObject(parent) {
	this->node = node;
	onode = new QSGOpacityNode;
	gnode = new QSGGeometryNode;
	geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	geometry->setLineWidth(4);
	geometry->setDrawingMode(GL_TRIANGLE_STRIP);
	gnode->setGeometry(geometry);
	gnode->setFlag(QSGNode::OwnsGeometry);
	gnode->setMaterial(material);
	gnode->setFlag(QSGNode::OwnsMaterial);
	geometry->allocate(1);
}

CleanInstallAnimation::~CleanInstallAnimation() {
	timer->stop();
	node->removeChildNode(onode);
	onode->removeChildNode(gnode);
	gnode->setFlag(QSGNode::OwnsGeometry, false);
	delete geometry;
	gnode->setFlag(QSGNode::OwnsMaterial, false);
	delete gnode;
	delete onode;
}

void CleanInstallAnimation::progress() {
	float timeSinceStart = initialTime.msecsTo(QTime::currentTime());
	if (timeSinceStart >= ANIMATIONDURATION) {
		delete this;
	} else {
		lastTime = QTime::currentTime();
		progress(timeSinceStart);
	}
}

void CleanInstallAnimation::progress(float timeSinceStart) {
	int p = posCount - timeSinceStart / ANIMATIONDURATION * posCount;
	geometry->allocate(p);
	QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
	for (int i = 0; i < p; i++) {
		int index = posCount - i - 1;
		vertices[i].set(pos[index].x(), pos[index].y());
	}
	gnode->markDirty(QSGNode::DirtyGeometry);
}

void CleanInstallAnimation::addSegment(segment *s) {
	for (int i = 0; i < s->poscount; i+=2) {
		pos[posCount] = s->pos[i];
		posCount++;
	}
}

void CleanInstallAnimation::fireAnimation() {
	onode->appendChildNode(gnode);
	node->appendChildNode(onode);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(progress()));
	initialTime = QTime::currentTime();
	lastTime = initialTime;
	progress(0);
	timer->start(16);
}
