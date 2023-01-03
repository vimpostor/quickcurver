#ifndef UTILITY_H
#define UTILITY_H

#include <QColor>
#include <QDataStream>
#include <QPointF>
#include <QQuickView>
#include <QTime>
#include <QtGlobal>
#include <algorithm>
#include <map>
#include <memory>
#include <ranges>

/**
 * @brief Contains frequently used useful routines that are available for every class
 */
namespace Util {
double rand();
QPointF randQPointF();
int randInt(const int lower, const int upper);
QColor randColor();
const QColor getColor(const QString color);
/**
	 * @brief Material design colors
	 */
const std::map<QString, QColor> colors = {
	{"Red", QColor(0xF4, 0x43, 0x36)},
	{"Pink", QColor(0xE9, 0x1E, 0x63)},
	{"Purple", QColor(0x9C, 0x27, 0xB0)},
	{"Deep Purple", QColor(0x67, 0x3A, 0xB7)},
	{"Indigo", QColor(0x3F, 0x51, 0xB5)},
	{"Blue", QColor(0x21, 0x96, 0xF3)},
	{"Light Blue", QColor(0x03, 0xA9, 0xF4)},
	{"Cyan", QColor(0x00, 0xBC, 0xD4)},
	{"Teal", QColor(0x00, 0x96, 0x88)},
	{"Green", QColor(0x4C, 0xAF, 0x50)},
	{"Light Green", QColor(0x8B, 0xC3, 0x4A)},
	{"Lime", QColor(0xCD, 0xDC, 0x39)},
	{"Yellow", QColor(0xFF, 0xEB, 0x3B)},
	{"Amber", QColor(0xFF, 0xC1, 0x07)},
	{"Orange", QColor(0xFF, 0x98, 0x00)},
	{"Deep Orange", QColor(0xFF, 0x57, 0x22)},
	{"Brown", QColor(0x79, 0x55, 0x48)},
	{"Grey", QColor(0x9E, 0x9E, 0x9E)},
	{"Blue Grey", QColor(0x60, 0x7D, 0x8B)}};
QString expandIconName(const QString icon);
bool getBit(const uint8_t byte, const int pos);
void setBit(uint8_t &byte, const int pos, bool value);
QQuickView *getTextureGenerator();
qint64 getTimeDiff(const QTime &t);
float easeInOutSine(const float &a);

// std algorithm wrappers

/**
	 * @brief Accumulates all elements in a given container using the + operator
	 * @param cnt The container to accumulate elements from
	 * @param init The initial sum value
	 * @return The accumulated sum
	 */
template<typename Cnt_T, typename T>
T accumulate(Cnt_T &cnt, T init) {
	return std::accumulate(std::begin(cnt), std::end(cnt), init);
}

/**
	 * @brief Calculates the partial sum
	 * @param in The container to calculate the partial sum of
	 * @param out The container to save the result in
	 * @return Iterator to the element past the last element written
	 */
template<typename Cnt_T>
auto partial_sum(Cnt_T &in, Cnt_T &out) {
	return std::partial_sum(std::begin(in), std::end(in), std::begin(out));
}

// container serialization

/**
	 * @brief Serializes a container
	 * @param out The stream to serialize into
	 * @param cnt The container to serialize
	 */
template<typename Cnt_T, typename Cnt_T::iterator *it = nullptr>
void serializeCnt(QDataStream &out, Cnt_T &cnt) {
	out << static_cast<unsigned>(cnt.size());
	for (auto &i : cnt) {
		out << i;
	}
}

/**
	 * @brief Parses data from a stream into a container
	 * @param in The stream to parse from
	 * @param cnt The container to parse into
	 */
template<typename Cnt_T, typename Cnt_T::iterator *it = nullptr>
void parseCnt(QDataStream &in, Cnt_T &cnt) {
	unsigned size;
	in >> size;
	cnt.resize(size);
	for (auto &i : cnt) {
		in >> i;
	}
}
}

#endif // UTILITY_H
