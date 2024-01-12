#include "item.hpp"

#include <QPainter>
#include <quartz/codepoints.hpp>

#define SIZE 12
#define FADEDURATION 256

/**
 * @brief Constructs a new Item instance
 * @param parentNode The parent node in the scene graph
 * @param iconName The path to the icon used as a texture
 * @param allowedUsers The allowed users for this Item
 * @param pos The location of this Item
 * @param window The window to render in
 */
Item::Item(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *window) {
	this->parentNode = parentNode;
	this->iconName = iconName;
	this->allowedUsers = allowedUsers;
	this->pos = pos;

	color = getColor();
	imgNode = window->createImageNode();
	imgNode->setFiltering(QSGTexture::Linear);
	imgNode->setMipmapFiltering(QSGTexture::Linear);
	initTexture(window);
	imgNode->setTexture(texture.get());
	startFade(true);
	fade();
	parentNode->appendChildNode(imgNode);
}

Item::~Item() {
	parentNode->removeChildNode(imgNode);
	delete imgNode;
}

/**
 * @brief Performs all updates on this Item
 */
void Item::update() {
	// check if needs to fade
	if (fadeStart.isValid()) {
		fade();
	}
	// check if this item should be deactivated
	if (active && unUseTime.isValid() && QTime::currentTime() > unUseTime) {
		defuse();
	}
}

/**
 * @brief Renders the Item inactive
 */
void Item::defuse() {
	if (active) {
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
		unUseTime = QTime::currentTime().addMSecs(activatedTime);
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
	imgNode->setRect(this->pos.x() - SIZE * factor, this->pos.y() - SIZE * factor, 2 * SIZE * factor, 2 * SIZE * factor);
	imgNode->markDirty(QSGNode::DirtyGeometry);
	if (actualDuration > FADEDURATION) {
		fadeStart = QTime();
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
 * @param window The window to create the texture in
 */
void Item::initTexture(QQuickWindow *window) {
	if (Settings::get()->getOffscreen()) {
		return;
	}
	constexpr const int res = 48;
	QImage img = QImage(res, res, QImage::Format_RGB32);
	img.fill(color); // fill with background color
	QPainter painter(&img);
	QFont font {"Material Symbols Outlined"};
	font.setPixelSize(res);
	painter.setFont(font);
	painter.drawText(QRect(0, 0, res, res), Qt::AlignCenter, Codepoints::get()->icon(iconName));

	// create the texture
	assert(window);
	texture = std::unique_ptr<QSGTexture>(window->createTextureFromImage(img, QQuickWindow::TextureHasMipmaps));
	assert(texture);
	texture->setFiltering(QSGTexture::Linear);
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
	fadeIn = in;
	fadeStart = QTime::currentTime();
}

/**
 * @brief Applies an effect to all affected Curver instances
 *
 * This can be the Item::use() or Item::unUse() routine.
 */
void Item::applyToAffected(void (Item::*method)(Curver *)) {
	switch (allowedUsers) {
	case AllowedUsers::ALLOW_ALL:
		std::ranges::for_each(PlayerModel::get()->getCurvers(), [&](auto &curver) { (this->*method)(curver.get()); });
		break;
	case AllowedUsers::ALLOW_OTHERS:
		std::ranges::for_each(PlayerModel::get()->getCurvers(), [&](auto &curver) { if (curver.get() != this->collector) (this->*method)(curver.get()); });
		break;
	case AllowedUsers::ALLOW_COLLECTOR:
		(this->*method)(collector);
		break;
	}
}
