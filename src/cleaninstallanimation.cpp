#include "cleaninstallanimation.hpp"

#define ANIMATION_DURATION 300

/**
 * @brief Triggers the cleaninstall animation
 * @param newSegments The current state of segments in the Curver object
 *
 * This will take ownership of all segments from the Curver object.
 * This method automatically removes all segments from the Curver object.
 */
void CleaninstallAnimation::trigger(std::vector<std::unique_ptr<Segment>> &newSegments) {
	if (newSegments.size() == 0) {
		// do not spawn an animation, if there is nothing to animate
		return;
	}
	for (auto it = newSegments.begin(); it != newSegments.end(); ++it) {
		segments.push_back(std::move(*it));
	}
	newSegments.clear();
	// calculate total amount of points
	sizeCache.resize(segments.size());
	pointsDeleted.resize(segments.size());
	for (size_t i = 0; i < segments.size(); ++i) {
		sizeCache[i] = segments[i]->getSegmentSize();
		pointsDeleted[i] = 0;
	}
	totalSize = Util::accumulate(sizeCache, 0);
	initialTime = QTime::currentTime();
}

/**
 * @brief Updates the animation
 */
void CleaninstallAnimation::progress() {
	if (initialTime.isNull()) {
		return;
	}
	const float timeSinceStart = initialTime.msecsTo(QTime::currentTime());
	const float factor = timeSinceStart / ANIMATION_DURATION;
	const float easedFactor = Util::easeInOutSine(factor);
	const size_t pos = totalSize * easedFactor;
	if (factor > 1) {
		segments.clear();
		sizeCache.clear();
		totalSize = 0;
		initialTime = QTime();
		return;
	}
	size_t pointsToDelete = pos;
	size_t segmentIndex;
	for (segmentIndex = 0; segmentIndex < sizeCache.size() && pointsToDelete >= sizeCache[segmentIndex]; ++segmentIndex) {
		pointsToDelete -= sizeCache[segmentIndex];
		segments[segmentIndex]->clear();
		pointsDeleted[segmentIndex] = sizeCache[segmentIndex];
	}
	if (segmentIndex >= segments.size()) {
		// whoops, out of bounds, so no explosion for us today
		return;
	}

	const auto pointsAlreadyDeleted = pointsToDelete;
	pointsToDelete -= pointsDeleted[segmentIndex];
	// remove points in an interval making an explosion every time
	constexpr size_t explosionInterval = 16;
	for (; pointsToDelete >= explosionInterval; pointsToDelete -= explosionInterval) {
		if (const auto p = segments[segmentIndex]->getFirstPos()) {
			emit spawnExplosion(p.value());
		}
		segments[segmentIndex]->popPoints(explosionInterval);
	}
	// remove the remaining few points without explosion
	segments[segmentIndex]->popPoints(pointsToDelete);
	pointsDeleted[segmentIndex] = pointsAlreadyDeleted;
}
