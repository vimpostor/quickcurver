#pragma once

#include <QImage>
#include <QObject>
#include <QPointF>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGTextureMaterial>

#include "curver.hpp"
#include "models/playermodel.hpp"
#include "util.hpp"

/**
 * @brief An Item that can be collected by a Curver and has certain effects
 */
class Item : public QObject {
	Q_OBJECT
public:
	/**
	 * @brief The affected Curver instances when an Item is triggered
	 */
	enum class AllowedUsers {
		ALLOW_ALL, // blue
		ALLOW_OTHERS, // red
		ALLOW_COLLECTOR // green
	};

	explicit Item(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos, QQuickWindow *window);
	~Item();

	void update();
	void defuse();
	void trigger(std::unique_ptr<Curver> &collector);
	bool isInRange(QPointF p) const;

	/**
	 * @brief The sequence number of this Item
	 *
	 * This property uniquely determines an Item for network communication
	 */
	unsigned int sequenceNumber = 0;
protected slots:
	void fade();
	void deactivate();
protected:
	virtual void use(Curver *);
	virtual void unUse(Curver *);
	QColor getColor() const;
	void initTexture(QQuickWindow *window);
	void startFade(bool in = true);
	void applyToAffected(void (Item::*method)(Curver *curver));

	/**
	 * @brief The parent node in the scene graph
	 */
	QSGNode *parentNode;
	/**
	 * @brief The path to the icon that is used as a texture
	 */
	QString iconName;
	/**
	 * @brief The affected Curver instances, once this Item instance is triggered
	 */
	AllowedUsers allowedUsers;
	/**
	 * @brief The location of the Item
	 */
	QPointF pos;
	/**
	 * @brief The amount of time that this Item stays activated before being deactivated in ms
	 *
	 * If the Item does not have to be deactivated, this value must be 0.
	 */
	int activatedTime = 0;
	/**
	 * @brief The Curver that collected the Item
	 */
	Curver *collector;
	/**
	 * @brief The texture of this Item
	 */
	std::unique_ptr<QSGTexture> texture;
	/**
	 * @brief The node displaying this Item in the scene graph
	 */
	QSGImageNode *imgNode;
	/**
	 * @brief The color of the Item
	 */
	QColor color;
	/**
	 * @brief The time when this Item should deactivate after it was triggered
	 *
	 * If the item wasn't used yet, this is the null time.
	 */
	QTime unUseTime;
	/**
	 * @brief The point of time that the last fade began
	 *
	 * If there is no fade going on right now, this is the null time.
	 */
	QTime fadeStart;
	/**
	 * @brief Whether the Item is currently fading in or out.
	 *
	 * If the Item is not fading in or out at all, this can be any value.
	 */
	bool fadeIn;
	/**
	 * @brief Whether the effect of the Item is currently active
	 */
	bool active = false;
};
