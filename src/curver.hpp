#ifndef QCURVER_H
#define QCURVER_H
#include <QQuickItem>
#include <QSGFlatColorMaterial>
#include <QSGNode>
#include <QTime>

#include "cleaninstallanimation.hpp"
#include "explosion.hpp"
#include "headnode.hpp"
#include "segment.hpp"
#include "settings.hpp"

/**
 * @brief The Curver class represents a player and all the segments belonging to the player
 *
 * This class stores every data of a player including all segments.
 */
class Curver : public QObject {
	Q_OBJECT
public:
	/**
	 * @brief Determines the rotation of a Curver object
	 */
	enum class Rotation {
		ROTATE_LEFT,
		ROTATE_NONE,
		ROTATE_RIGHT
	};
	/**
	 * @brief Determines the controller of a Curver object
	 */
	enum class Controller {
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
	const std::vector<std::unique_ptr<Segment>> &getSegments() const;
	QPointF getPos() const;
	QPointF getDirection() const;
	float getAngle() const;
	bool isChangingSegment() const;

	void processKey(Qt::Key key, bool release = false);
	void start();
	void progress(int deltat, std::vector<std::unique_ptr<Curver>> &curvers);
	bool checkForIntersection(std::vector<std::unique_ptr<Curver>> &curvers, QPointF a, QPointF b) const;
	void checkForWall();
	void cleanInstall();
	void increaseScore();
	void resetRound();
	void setAlive(const bool alive);
	bool isAlive() const;
	void appendPoint(const QPointF pos, const bool changingSegment);
	void prepareSegmentEvent(bool changingSegment, int lower, int upper);
	void spawnExplosion(QPointF location, float radius = 1.0);

	/**
	 * @brief The username of the Curver
	 */
	QString userName = "Player";
	/**
	 * @brief The amount of points gained during the current round
	 */
	int roundScore = 0;
	/**
	 * @brief The amount of total points gained
	 */
	int totalScore = 0;
	/**
	 * @brief The current rotation
	 */
	Rotation rotation = Rotation::ROTATE_NONE;
	/**
	 * @brief The current controller of this Curver
	 */
	Controller controller = Controller::CONTROLLER_LOCAL;
	/**
	 * @brief The current velocity in pixels per millisecond
	 */
	float velocity = 0.125;
	/**
	 * @brief The rotational velocity in radian per millisecond
	 */
	float rotateVelocity = 0.0039062f;
	/**
	 * @brief Determines if the head is visible at the moment
	 */
	bool headVisible = true;
signals:
	/**
	 * @brief Emitted, when the Curver died due to collision
	 */
	void died();
private slots:
private:
	void rotate(float radians);
	void die();

	/**
	 * @brief The parent node in the scene graph
	 */
	QSGNode *parentNode;
	/**
	 * @brief The color of this Curver
	 */
	QColor color = Qt::black;
	/**
	 * @brief The material in use with the color determined by Curver::color
	 */
	QSGFlatColorMaterial material;
	/**
	 * @brief A vector containing all segments of this Curver
	 */
	std::vector<std::unique_ptr<Segment>> segments;
	/**
	 * @brief The node representing the head of this Curver
	 */
	std::unique_ptr<HeadNode> headNode;
	/**
	 * @brief The current line thickness
	 */
	float thickness = 4;
	/**
	 * @brief The vector pointing at the direction that this Curver is heading at
	 */
	QPointF direction;
	/**
	 * @brief The angle of the current rotation in radian
	 */
	float angle = 0;
	/**
	 * @brief The current position
	 */
	QPointF lastPos = QPointF(0, 0);
	/**
	 * @brief The position before Curver::lastPos
	 */
	QPointF secondLastPos;
	/**
	 * @brief The key triggering the counter clock-wise rotation
	 */
	Qt::Key leftKey = Qt::Key_Left;
	/**
	 * @brief The key triggering the clock-wise rotation
	 */
	Qt::Key rightKey = Qt::Key_Right;
	/**
	 * @brief Determines, whether the Curver is changing segments at the moment
	 */
	bool changingSegment = false;
	/**
	 * @brief The time of the next planned segment event
	 *
	 * A segment event can be the spawn of a new segment or leaving the current segment.
	 */
	QTime nextSegmentEvent;
	/**
	 * @brief Decides whether the Curver is alive at the moment
	 */
	bool alive = true;
	/**
	 * @brief Decides whether the Curver was previously changing the segment
	 *
	 * This property is used only by the Client to decide, when to trigger segment events
	 *
	 * Unused by Server.
	 */
	bool oldChangingSegment = true;
	/**
	 * @brief An object responsible for the cleaninstall animation
	 *
	 * Note, that this object will take ownership of all segments when triggered
	 */
	CleaninstallAnimation cleaninstallAnimation;
};

bool operator<(const std::unique_ptr<Curver> &l, const std::unique_ptr<Curver> &r);

#endif // QCURVER_H
