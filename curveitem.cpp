#include "curveitem.h"

CurveItem::CurveItem(QSGNode *node) {
	this->node = node;
	pos = QPointF(segment::randInt(10, 990), segment::randInt(10,990));
	int r = segment::randInt(0,2);
	switch (r) {
	case 0:
		color = Qt::green;
		break;
	case 1:
		color = Qt::red;
		break;
	default:
		color = Qt::blue;
		break;
	}
	gnode = new QSGGeometryNode;
	geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	geometry->setLineWidth(4);
	geometry->setDrawingMode(GL_LINE_STRIP);
	gnode->setGeometry(geometry);
	gnode->setFlag(QSGNode::OwnsGeometry);
	QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
	material->setColor(color);
	gnode->setMaterial(material);
	gnode->setFlag(QSGNode::OwnsMaterial);
	geometry->allocate(4);
	vertices = geometry->vertexDataAsPoint2D();
	vertices[0].set(pos.x()-10,pos.y()-10);
	vertices[1].set(pos.x()-10,pos.y()+10);
	vertices[2].set(pos.x()+10, pos.y()-10);
	vertices[3].set(pos.x()+10, pos.y()+10);
	node->appendChildNode(gnode);
}

CurveItem::~CurveItem() {
	//destruct
	node->removeChildNode(gnode);
}

int CurveItem::getSize() {
	return SIZE;
}

QPointF CurveItem::getPos() {
	return pos;
}

bool CurveItem::testCollision(QPointF p) {
	QPointF difference = p-pos;
	if (qAbs(difference.x()) < SIZE && qAbs(difference.y()) < SIZE) {
		return true;
	} else {
		return false;
	}
}

QColor CurveItem::getColor() {
	return color;
}

void CurveItem::useItem(int playerCount, QCurver **curver, QCurver *collector) {
	this->playerCount = playerCount;
	this->curver = curver;
	this->collector = collector;
	if (color == Qt::green) {
		useMyself();
	} else if (color == Qt::red) {
		useOthers();
	} else {
		useAll();
	}
	node->removeChildNode(gnode);
	if (deUseTime != 0) {
		deuseIn(deUseTime);
	} else {
		delete this;
	}
}

void CurveItem::useMyself() {
	use(collector);
}

void CurveItem::useOthers() {
	for (int i = 0; i < playerCount; i++) {
		if (curver[i] != collector) {
			use(curver[i]);
		}
	}
}

void CurveItem::useAll() {
	for (int i = 0; i < playerCount; i++) {
		use(curver[i]);
	}
}

void CurveItem::stopTimer() {
	timer->stop();
}

void CurveItem::deuseMyself() {
	deuse(collector);
}

void CurveItem::deuseAll() {
	for (int i = 0; i < playerCount; i++) {
		deuse(curver[i]);
	}
}

void CurveItem::deuseOthers() {
	for (int i = 0; i < playerCount; i++) {
		if (curver[i] != collector) {
			deuse(curver[i]);
		}
	}
}

void CurveItem::deuseIn(int msecs) {
	timer = new QTimer;
	if (color == Qt::green) {
		timer->singleShot(deUseTime, this, SLOT(deuseMyself()));
	} else if (color == Qt::red) {
		timer->singleShot(deUseTime, this, SLOT(deuseOthers()));
	} else { //blue
		timer->singleShot(deUseTime, this, SLOT(deuseAll()));
	}
}

//the following methods are implemented later in subclasses
void CurveItem::use(QCurver *curver) {
}

void CurveItem::deuse(QCurver *curver) {
}
