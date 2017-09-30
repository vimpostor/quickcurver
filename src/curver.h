#ifndef QCURVER_H
#define QCURVER_H
#include <QQuickItem>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QTime>

#include "segment.h"
#include "explosion.h"
#include "headnode.h"
#include "settings.h"


class Curver : public QObject {
	Q_OBJECT
public:
	enum Rotation {
		ROTATE_LEFT,
		ROTATE_NONE,
		ROTATE_RIGHT
	};
	enum Controller {
		CONTROLLER_LOCAL,
		CONTROLLER_REMOTE,
		CONTROLLER_BOT
	};

	explicit Curver(QSGNode *parentNode);
	~Curver();

	void setColor(const QColor color);
	QColor getColor() const;
	void setLeftKey(const Qt::Key key);
	Qt::Key getLeftKey() const;
	void setRightKey(const Qt::Key key);
	Qt::Key getRightKey() const;
	const std::vector<std::unique_ptr<Segment> > &getSegments() const;
	QPointF getPos() const;
	QPointF getDirection() const;
	float getAngle() const;
	bool isChangingSegment() const;

	void processKey(Qt::Key key, bool release = false);
	void start();
	bool checkForIntersection(std::vector<std::unique_ptr<Curver> > &curvers, QPointF a, QPointF b) const;
	void checkForWall();
	void cleanInstall();
	void increaseScore();
	void resetRound();
	bool isAlive() const;
	void appendPoint(const QPointF pos, const bool changingSegment);
	void prepareSegmentEvent(bool changingSegment, int lower, int upper);

	QString userName = "Player";
	int roundScore = 0;
	int totalScore = 0;
	enum Rotation rotation = ROTATE_NONE;
	enum Controller controller = CONTROLLER_LOCAL;
	float velocity = 0.125;
	float rotateVelocity = 0.0039062f;
signals:
	void died();
public slots:
	void progress(int deltat, std::vector<std::unique_ptr<Curver> > &curvers);
private slots:
private:
	void rotate(float radians);
	void die();

	QSGNode *parentNode;
	QColor color = Qt::black;
	QSGFlatColorMaterial material;
	std::vector<std::unique_ptr<Segment>> segments;
	std::unique_ptr<HeadNode> headNode;
	float thickness = 4;
	QPointF direction;
	float angle = 0;
	QPointF lastPos = QPointF(0,0);
	QPointF secondLastPos;
	Qt::Key leftKey = Qt::Key_Left;
	Qt::Key rightKey = Qt::Key_Right;
	bool changingSegment = false;
	QTime nextSegmentEvent;
	bool alive = true;
	bool oldChangingSegment = true;
};

#endif // QCURVER_H
