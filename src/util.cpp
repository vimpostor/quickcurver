#include "util.h"

/**
 * @brief Initializes the random seed
 */
void Util::init()
{
	qsrand(static_cast<uint>(QDateTime::currentMSecsSinceEpoch()));
}

/**
 * @brief Returns a random number between 0 and 1
 * @return A random number between 0 and 1
 */
double Util::rand()
{
	return static_cast<double>(qrand()) / RAND_MAX;
}

/**
 * @brief Returns a random QPointF with values between 0 and 1 each
 * @return A random QPointF
 */
QPointF Util::randQPointF()
{
	return QPointF(rand(), rand());
}

/**
 * @brief Returns a random integer in the given range
 * @param lower Lower boundary
 * @param upper Upper boundary
 * @return A random integer in the given range
 */
int Util::randInt(const int lower, const int upper)
{
	return lower + rand() * (upper - lower);
}

/**
 * @brief Returns a random Material design color
 * @return A random color
 */
QColor Util::randColor()
{
	auto it = colors.begin();
	std::advance(it, randInt(0, static_cast<int>(colors.size()) - 1));
	return it->second;
}

/**
 * @brief Returns a Material design color
 * @param color The color to look up
 * @return The Material design color
 */
const QColor Util::getColor(const QString color)
{
	return colors.find(color)->second;
}

/**
 * @brief Expands the icon name, so that the icon name can be used from the Qt resource system
 *
 * Example icon name to expand: "action/favorite"
 * @param icon The icon name to expand
 * @return The expanded icon name, that is ready to use from the Qt resource system
 */
QString Util::expandIconName(const QString icon)
{
	return QStringLiteral(":/liri.io/imports/Fluid/Controls/icons/%1.svg").arg(icon);
}

/**
 * @brief Returns the bit at the position \a pos in \a byte counting from right
 * @param byte The byte to extract the bit from
 * @param pos The position of the bit
 * @return The extracted bit
 */
bool Util::getBit(const uint8_t byte, const int pos)
{
	return (byte >> pos) % 2;
}

/**
 * @brief Sets the bit at position \a pos in \a byte to \a value
 * @param byte The byte to set a bit in
 * @param pos The position of the bit
 * @param value The new value of the bit
 */
void Util::setBit(uint8_t& byte, const int pos, bool value)
{
	byte |= value << pos;
}
