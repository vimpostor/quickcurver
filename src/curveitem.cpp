#include "curveitem.h"
#include <QSvgRenderer>
#include <QPainter>

CurveItem::CurveItem(QSGNode *node, QQuickView *view, QString iconPath, QColor color, QPointF pos) {
    this->node = node;
    this->pos = pos;
    this->color = color;
    initCurveItem(view, iconPath);
}

CurveItem::CurveItem(QSGNode *node, QQuickView *view, int fieldsize, QString iconPath, bool redAllowed, bool greenAllowed, bool blueAllowed, QPointF *pos) {
    this->node = node;
    if (pos == NULL) {
        this->pos = QPointF(segment::randInt(10, fieldsize-10), segment::randInt(10,fieldsize-10));
    } else {
        this->pos = QPointF(pos->x(), pos->y());
    }
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
        color = ITEMGREENCOLOR;
        break;
    case 1:
        color = ITEMREDCOLOR;
        break;
    default:
        color = ITEMBLUECOLOR;
        break;
    }
    initCurveItem(view, iconPath);
}

void CurveItem::initCurveItem(QQuickView *view, QString iconPath) {
    gnode = new QSGGeometryNode;
    geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0);
    geometry->setDrawingMode(GL_TRIANGLE_STRIP);
    gnode->setGeometry(geometry);
    gnode->setFlag(QSGNode::OwnsGeometry);
    material = new QSGTextureMaterial();
    QImage img = QImage(2*SIZE, 2*SIZE, QImage::Format_RGB16);
    img.fill(color); // icon background color
    QSvgRenderer renderer(iconPath);
    QPainter painter(&img);
    renderer.render(&painter); // paint the icon on top of it
    material = new QSGTextureMaterial;
    texture = view->createTextureFromImage(img);
    texture->setMipmapFiltering(QSGTexture::Linear);
    texture->bind();
    material->setTexture(texture);
    gnode->setMaterial(material);
    gnode->setFlag(QSGNode::OwnsMaterial);
    geometry->allocate(4);
    vertices = geometry->vertexDataAsTexturedPoint2D();
    vertices[0].set(this->pos.x()-SIZE,this->pos.y()-SIZE,0,0);
    vertices[1].set(this->pos.x()+SIZE,this->pos.y()-SIZE,1,0);
    vertices[2].set(this->pos.x()-SIZE,this->pos.y()+SIZE,0,1);
    vertices[3].set(this->pos.x()+SIZE,this->pos.y()+SIZE,1,1);
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
    if (color == ITEMGREENCOLOR) {
		useMyself();
    } else if (color == ITEMREDCOLOR) {
		useOthers();
    } else if (color == ITEMBLUECOLOR) {
		useAll();
    } else  {
        qDebug() << "Unexpected color in useItem()";
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
    if (color == ITEMGREENCOLOR) {
		timer->singleShot(deUseTime, this, SLOT(deuseMyself()));
    } else if (color == ITEMREDCOLOR) {
		timer->singleShot(deUseTime, this, SLOT(deuseOthers()));
    } else if (color == ITEMBLUECOLOR) {
		timer->singleShot(deUseTime, this, SLOT(deuseAll()));
    } else {
        qDebug() << "Unexpected color in deuseIn()";
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

QString CurveItem::getIconName() {
    return "";
}
