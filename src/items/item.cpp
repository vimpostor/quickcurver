#include "item.hpp"

#include <QPainter>
#include <QSvgRenderer>

#define SIZE 12
#define FADEDURATION 256

/**
 * @brief Constructs a new Item instance
 * @param parentNode The parent node in the scene graph
 * @param iconPath The path to the icon used as a texture
 * @param allowedUsers The allowed users for this Item
 * @param pos The location of this Item
 */
Item::Item(QSGNode *parentNode, QString iconPath, AllowedUsers allowedUsers, QPointF pos) {
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
	connect(&fadeTimer, &QTimer::timeout, this, &Item::fade);
	startFade(true);
	fade();
	parentNode->appendChildNode(&geoNode);
}

/**
 * @brief Renders the Item inactive
 */
void Item::defuse() {
	if (active) {
		unUseTimer.stop();
		deactivate();
	}
}

/**
 * @brief Triggers the Item
 * @param collector The collecting Curver
 */
void Item::trigger(std::unique_ptr<Curver> &collector) {
	this->collector = collector.get();

	applyToAffected(&Item::use);
	active = true;
	if (this->activatedTime != 0) {
		// has to be deactivated
		unUseTimer.singleShot(activatedTime, this, &Item::deactivate);
	}
	startFade(false);
}

/**
 * @brief Fades the Item in or out according to how much time passed by
 */
void Item::fade() {
	float actualDuration = fadeStart.msecsTo(QTime::currentTime());
	float factor = !fadeIn + (fadeIn - !fadeIn) * actualDuration / FADEDURATION;
	factor = qMin(1.f, qMax(0.f, factor)); // 0 <= factor <= 1
	QSGGeometry::TexturedPoint2D *vertices = geometry.vertexDataAsTexturedPoint2D();
	vertices[0].set(this->pos.x() - SIZE * factor, this->pos.y() - SIZE * factor, 0, 0);
	vertices[1].set(this->pos.x() + SIZE * factor, this->pos.y() - SIZE * factor, 1, 0);
	vertices[2].set(this->pos.x() - SIZE * factor, this->pos.y() + SIZE * factor, 0, 1);
	vertices[3].set(this->pos.x() + SIZE * factor, this->pos.y() + SIZE * factor, 1, 1);
	geoNode.markDirty(QSGNode::DirtyGeometry);
	if (actualDuration > FADEDURATION) {
		fadeTimer.stop();
		if (!fadeIn && activatedTime == 0) {
			// parentNode->removeChildNode(&geoNode);
		}
	}
}

/**
 * @brief Triggered, when the Item was used and a non permanent effect of the Item must be stopped.
 *
 * Calls Item::unUse() on all affected Curver instances
 */
void Item::deactivate() {
	active = false;
	applyToAffected(&Item::unUse);
	// parentNode->removeChildNode(&geoNode);
}

/**
 * @brief The immediate effect when the Item is triggered
 */
void Item::use(Curver *) {
}

/**
 * @brief The antidote for Item::use().
 *
 * This is triggered, when the Item effect should be deactived again.
 */
void Item::unUse(Curver *) {
}

/**
 * @brief Returns the color of the Item
 * @return The color
 */
QColor Item::getColor() const {
	switch (allowedUsers) {
	case AllowedUsers::ALLOW_ALL:
		return Util::getColor("Blue");
	case AllowedUsers::ALLOW_OTHERS:
		return Util::getColor("Red");
	default:
		return Util::getColor("Green");
	}
}

/**
 * @brief Initializes the texture of the Item
 */
void Item::initTexture() {
	if (Settings::getSingleton().getOffscreen()) {
		return;
	}
	QSvgRenderer renderer(iconPath);
	const auto size = renderer.defaultSize();
	constexpr int minRasteriseSize = SIZE * 8;
	QImage img = QImage(std::max(minRasteriseSize, size.width()), std::max(minRasteriseSize, size.height()), QImage::Format_RGB32);
	img.fill(color); // fill with background color
	QPainter painter(&img);
	renderer.render(&painter); // paint the icon on top of it
	texture = std::unique_ptr<QSGTexture>(Util::getTextureGenerator()->createTextureFromImage(img));
	texture->setMipmapFiltering(QSGTexture::Linear);
}

/**
 * @brief Checks if a given point is in trigger range of the Item
 * @param p The point to check for
 * @return \c True, iif \a p is in range
 */
bool Item::isInRange(QPointF p) const {
	QPointF diff = p - pos;
	if (qAbs(diff.x()) < SIZE && qAbs(diff.y()) < SIZE) {
		return true;
	}
	return false;
}

/**
 * @brief Starts a visual fade of the Item
 * @param in Whether to fade in or out
 */
void Item::startFade(bool in) {
	fadeStart = QTime::currentTime();
	fadeIn = in;
	fadeTimer.start(16);
}

/**
 * @brief Applies an effect to all affected Curver instances
 *
 * This can be the Item::use() or Item::unUse() routine.
 */
void Item::applyToAffected(void (Item::*method)(Curver *)) {
	switch (allowedUsers) {
	case AllowedUsers::ALLOW_ALL:
		std::ranges::for_each(PlayerModel::getSingleton().getCurvers(), [&](auto &curver) { (this->*method)(curver.get()); });
		break;
	case AllowedUsers::ALLOW_OTHERS:
		std::ranges::for_each(PlayerModel::getSingleton().getCurvers(), [&](auto &curver) { if (curver.get() != this->collector) (this->*method)(curver.get()); });
		break;
	case AllowedUsers::ALLOW_COLLECTOR:
		(this->*method)(collector);
		break;
	}
}
