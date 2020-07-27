#ifndef CLEANINSTALLANIMATION_H
#define CLEANINSTALLANIMATION_H

#include <QObject>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QTimer>
#include <QTime>
#include <memory>

#include "segment.h"
#include "util.h"

/**
 * @brief An animated event representing a Curver cleaninstall
 *
 * Takes ownership of all segments of the old Curver object
 */
class CleaninstallAnimation : public QObject
{
	Q_OBJECT
public:
	explicit CleaninstallAnimation(QObject *parent = nullptr);
	void trigger(std::vector<std::unique_ptr<Segment>>& newSegments);
signals:
	/**
	 * @brief Request to spawn an explosion
	 * @param location The position of the explosion
	 */
	void spawnExplosion(QPointF location);
private:
	/**
	 * @brief The timer updating the animation
	 */
	QTimer timer;
	/**
	 * @brief The point in time that the animation was triggered with trigger()
	 */
	QTime initialTime;
	/**
	 * @brief The old segments to be faded out
	 *
	 * trigger() will automatically take ownership of the old segments from a Curver object
	 */
	std::vector<std::unique_ptr<Segment>> segments;
	/**
	 * @brief A cache representing the size of the segments at the point of the animation begin
	 */
	std::vector<size_t> sizeCache;
	/**
	 * @brief A cache representing the amount of points already deleted for a given segment
	 */
	std::vector<size_t> pointsDeleted;
	/**
	 * @brief The total size over all segments
	 */
	size_t totalSize;
private slots:
	void progress();
};

#endif // CLEANINSTALLANIMATION_H
