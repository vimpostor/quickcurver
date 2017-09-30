#ifndef CURVEITEM_H
#define CURVEITEM_H
#include <QPointF>
#include <QSGNode>
#include <QSGTextureMaterial>
#include <QObject>
#include <QQuickWindow>
#include <QOpenGLTexture>
#include <QQuickWindow>
#include <QTimer>
#include <QImage>
#include <QQuickView>

#include "curver.h"
#include "util.h"
#include "models/playermodel.h"

class Item : public QObject
{
	Q_OBJECT
public:
	enum AllowedUsers {
		ALLOW_ALL, // blue
		ALLOW_OTHERS, // red
		ALLOW_COLLECTOR // green
	};

	explicit Item(QSGNode *parentNode, QString iconPath, AllowedUsers allowedUsers, QPointF pos);

	void defuse();
	void trigger(std::unique_ptr<Curver> &collector);
	bool isInRange(QPointF p) const;

	unsigned int sequenceNumber = 0;
protected slots:
	void fade();
	void deactivate();
protected:
	virtual void use(Curver *);
	virtual void unUse(Curver *);
	QColor getColor() const;
	void initTexture();
	void startFade(bool in = true);
	void applyToAffected(void (Item::*method)(Curver *curver));

	QSGNode *parentNode;
	QString iconPath;
	AllowedUsers allowedUsers;
	QPointF pos;
	int activatedTime = 0;
	Curver *collector;
	QSGTextureMaterial material;
	static QQuickView *textureGenerator;
	std::unique_ptr<QSGTexture> texture;
	QSGGeometryNode geoNode;
	QSGGeometry geometry = QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0);
	QColor color;
	QTimer unUseTimer;
	QTimer fadeTimer;
	QTime fadeStart;
	bool fadeIn;
	bool active = false;
};

#endif // CURVEITEM_H
