#include "segment.h"

segment::segment(QColor color, QSGNode *node, QSGFlatColorMaterial *material) {
	this->node = node;
	this->color = color;
	nodeMutex.lock();
	gnode = new QSGGeometryNode;
	geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	geometry->setLineWidth(thickness);
	geometry->setDrawingMode(GL_LINE_STRIP);
	gnode->setGeometry(geometry);
	gnode->setFlag(QSGNode::OwnsGeometry);
	gnode->setMaterial(material);
	gnode->setFlag(QSGNode::OwnsMaterial);
	geometry->allocate(1);
	node->appendChildNode(gnode);
	nodeMutex.unlock();
}

segment::~segment() {
	node->removeChildNode(gnode);
	delete geometry;
//	delete gnode;
}

void segment::initRand() {
	qsrand(QTime::currentTime().msecsSinceStartOfDay());
}

void segment::appendPoint(QPointF addedPoint) {
	nodeMutex.lock();
	pos[poscount] = addedPoint;
	poscount++;
	geometry->allocate(poscount);
	QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
	for (int i = 0; i < poscount; i++) {
		vertices[i].set(pos[i].x(), pos[i].y());
	}
	gnode->markDirty(QSGNode::DirtyGeometry);
	nodeMutex.unlock();
}

QPointF segment::getLastPoint() {
	return pos[poscount-1];
}

int segment::randInt(int min, int max) {
	return ((qrand() % ((max + 1) - min)) + min);
}

float segment::randFloat() {
	return (float) randInt(0,10000)/10000;
}

bool segment::checkforIntersection(QPointF a, QPointF b) {
	float bax, dcx, acy, dcy, bay, baxdcy, s, t;
	for (int i = 0; i < poscount-3; i++) { //we dont check with the last line, because that could be the same line, as the one that emited intersection checking
		QPointF c = pos[i];
		QPointF d = pos[i+1];
		bax = b.x()-a.x();
		dcx = d.x()-c.x();
		acy = a.y()-c.y();
		dcy = d.y()-c.y();
		bay = b.y()-a.y();
		baxdcy = bax*dcy;
		s = ((c.x()-a.x())/bax+dcx*acy/baxdcy)/(1-dcx*bay/(bax*dcy));
		if (s < 0 || s > 1) {
			continue;
		} else {
			t = acy/dcy+s*bay/dcy;
			if (t >= 0 && t <= 1) {
				return true;
			}
		}
	}
	return false;
}
