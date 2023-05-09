#ifndef CLEANINSTALLANIMATION_H
#define CLEANINSTALLANIMATION_H

#include <QObject>
#include <QSGFlatColorMaterial>
#include <QSGNode>
#include <QTime>
#include <memory>

#include "segment.hpp"
#include "util.hpp"

/**
 * @brief An animated event representing a Curver cleaninstall
 *
 * Takes ownership of all segments of the old Curver object
 */
class CleaninstallAnimation : public QObject {
	Q_OBJECT
public:
	void trigger(std::vector<std::unique_ptr<Segment>> &newSegments);
	void progress();
signals:
	/**
	 * @brief Request to spawn an explosion
	 * @param location The position of the explosion
	 */
	void spawnExplosion(QPointF location);
private:
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
};

#endif // CLEANINSTALLANIMATION_H
