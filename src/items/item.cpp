#include "item.h"

#include <QSvgRenderer>
#include <QPainter>

#define SIZE 12
#define FADEDURATION 256

QQuickView *Item::textureGenerator = NULL;

Item::Item(QSGNode *parentNode, QString iconPath, AllowedUsers allowedUsers, QPointF pos)
{
	this->parentNode = parentNode;
	this->iconPath = iconPath;
	this->allowedUsers = allowedUsers;
	this->pos = pos;

	geometry.setDrawingMode(GL_TRIANGLE_STRIP);
	geoNode.setGeometry(&geometry);
	color = getColor();
	initTexture();
	material.setTexture(texture.get());
	geoNode.setMaterial(&material);
	geometry.allocate(4);
	connect(&fadeTimer, SIGNAL(timeout()), this, SLOT(fade()));
	startFade(true);
	fade();
	parentNode->appendChildNode(&geoNode);
}

void Item::defuse()
{
	if (active) {
		unUseTimer.stop();
		deactivate();
	}
}

void Item::trigger(std::unique_ptr<Curver> &collector)
{
	this->collector = collector.get();

	applyToAffected(&Item::use);
	active = true;
	if (this->activatedTime != 0) {
		// has to be deactivated
		unUseTimer.singleShot(activatedTime, this, SLOT(deactivate()));
	}
	startFade(false);
}

void Item::fade()
{
	float actualDuration = fadeStart.msecsTo(QTime::currentTime());
	float factor = !fadeIn + (fadeIn - !fadeIn) * actualDuration / FADEDURATION;
	factor = qMin(1.f, qMax(0.f, factor)); // 0 <= factor <= 1
	QSGGeometry::TexturedPoint2D *vertices = geometry.vertexDataAsTexturedPoint2D();
	vertices[0].set(this->pos.x() - SIZE*factor, this->pos.y() - SIZE*factor, 0, 0);
	vertices[1].set(this->pos.x() + SIZE*factor, this->pos.y() - SIZE*factor, 1, 0);
	vertices[2].set(this->pos.x() - SIZE*factor, this->pos.y() + SIZE*factor, 0, 1);
	vertices[3].set(this->pos.x() + SIZE*factor, this->pos.y() + SIZE*factor, 1, 1);
	geoNode.markDirty(QSGNode::DirtyGeometry);
	if (actualDuration > FADEDURATION) {
		fadeTimer.stop();
		if (!fadeIn && activatedTime == 0) {
//			parentNode->removeChildNode(&geoNode);
		}
	}
}

void Item::deactivate()
{
	active = false;
	applyToAffected(&Item::unUse);
//	parentNode->removeChildNode(&geoNode);
}

void Item::use(Curver *)
{
}

void Item::unUse(Curver *)
{
}

QColor Item::getColor() const
{
	switch (allowedUsers) {
	case AllowedUsers::ALLOW_ALL:
		return Util::getColor("Blue");
	case AllowedUsers::ALLOW_OTHERS:
		return Util::getColor("Red");
	default:
		return Util::getColor("Green");
	}
}

void Item::initTexture()
{
	if (textureGenerator == NULL) {
		// textureGenerator NULL, so create one
		textureGenerator = new QQuickView();
		// TODO: Free this at the end
	}
	QImage img = QImage(SIZE*2, SIZE*2, QImage::Format_RGB16);
	img.fill(color); // fill with background color
	QSvgRenderer renderer(iconPath);
	QPainter painter(&img);
	renderer.render(&painter); // paint the icon on top of it
	texture = std::unique_ptr<QSGTexture>(this->textureGenerator->createTextureFromImage(img));
	texture->setMipmapFiltering(QSGTexture::Linear);
	texture->bind();
}

bool Item::isInRange(QPointF p) const
{
	QPointF diff = p - pos;
	if (qAbs(diff.x()) < SIZE && qAbs(diff.y()) < SIZE) {
		return true;
	}
	return false;
}

void Item::startFade(bool in)
{
	fadeStart = QTime::currentTime();
	fadeIn = in;
	fadeTimer.start(16);
}

void Item::applyToAffected(void (Item::*method)(Curver *))
{
	switch (allowedUsers) {
	case AllowedUsers::ALLOW_ALL:
		Util::for_each(PlayerModel::getSingleton().getCurvers(), [&](auto &curver){ (this->*method)(curver.get()); });
		break;
	case AllowedUsers::ALLOW_OTHERS:
		Util::for_each(PlayerModel::getSingleton().getCurvers(), [&](auto &curver){ if (curver.get() != this->collector) (this->*method)(curver.get()); });
		break;
	case AllowedUsers::ALLOW_COLLECTOR:
		(this->*method)(collector);
		break;
	}
}
