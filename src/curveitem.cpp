#include "curveitem.h"

CurveItem::CurveItem(QSGNode *node, int fieldsize, bool greenAllowed, bool redAllowed, bool blueAllowed) {
	this->node = node;
    pos = QPointF(segment::randInt(10, fieldsize-10), segment::randInt(10,fieldsize-10));

	//find a random allowed color
	bool allowedColors[3] = {greenAllowed, redAllowed, blueAllowed};
	int allowedColorsSum = greenAllowed + redAllowed + blueAllowed;
	int r = segment::randInt(0, allowedColorsSum-1);
	int i = -1;
	int colorCount = -1;
	do {
		i++;
		colorCount++;
		for (; !allowedColors[i]; i++) {}; //this moves to the next possible color position
	} while (colorCount != r);
	switch (i) {
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
	geometry->setDrawingMode(GL_TRIANGLES);
	gnode->setGeometry(geometry);
	gnode->setFlag(QSGNode::OwnsGeometry);
	QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
	material->setColor(color);
//	QImage *img = new QImage(":/images/flash.png");
//	this->material = new QSGTextureMaterial;
//	texture = view->createTextureFromImage(*img);
//	texture->setMipmapFiltering(QSGTexture::Linear);
//	texture->setHorizontalWrapMode(QSGTexture::Repeat);
//	texture->setVerticalWrapMode(QSGTexture::Repeat);
//	this->material->setTexture(texture);
	gnode->setMaterial(material);
	gnode->setFlag(QSGNode::OwnsMaterial);
	geometry->allocate(6);
	vertices = geometry->vertexDataAsPoint2D();
	vertices[0].set(pos.x()-10,pos.y()-10);
	vertices[1].set(pos.x()-10,pos.y()+10);
	vertices[2].set(pos.x()+10, pos.y()-10);
	vertices[3].set(pos.x()+10, pos.y()+10);
	vertices[4].set(pos.x()-10, pos.y()+10);
	vertices[5].set(pos.x()+10, pos.y()-10);
	node->appendChildNode(gnode);
}

CurveItem::~CurveItem() {
	if (invisible == false) { //fixes an issue where simultaneously using an item and starting a new round would render the whole screen black
		node->removeChildNode(gnode);
		invisible = true;
	}
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
	invisible = true;
	if (deUseTime != 0) {
		deuseIn();
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

void CurveItem::deuseMyself() {
	if (collector->verifyCorrectRound(round)) { //are we still in the same round?
		deuse(collector);
	}
	delete this;
}

void CurveItem::deuseAll() {
	if (collector->verifyCorrectRound(round)) {
		for (int i = 0; i < playerCount; i++) {
			deuse(curver[i]);
		}
	}
	delete this;
}

void CurveItem::deuseOthers() {
	if (collector->verifyCorrectRound(round)) {
		for (int i = 0; i < playerCount; i++) {
			if (curver[i] != collector) {
				deuse(curver[i]);
			}
		}
	}
	delete this;
}

void CurveItem::deuseIn() {
	timer = new QTimer;
	if (color == Qt::green) {
		timer->singleShot(deUseTime, this, SLOT(deuseMyself()));
	} else if (color == Qt::red) {
		timer->singleShot(deUseTime, this, SLOT(deuseOthers()));
	} else { //blue
		timer->singleShot(deUseTime, this, SLOT(deuseAll()));
	}
}

void CurveItem::setRound(int round) {
	this->round = round;
}

//the following methods are implemented later in subclasses
void CurveItem::use(QCurver *curver) {
	(void) curver; //suppresses unused variable warning
}

void CurveItem::deuse(QCurver *curver) {
	(void) curver; //suppresses unused variable warning
}
