#include "settings.h"

Settings::Settings()
{
}

/**
 * @brief Returns the Settings singleton
 * @return The Settings singleton
 */
Settings &Settings::getSingleton()
{
	static Settings result;
	return result;
}

/**
 * @brief Sets the dimension of the game
 * @param dimension The new dimension
 */
void Settings::setDimension(QPoint dimension)
{
	this->dimension = dimension;
	emit widthChanged(dimension.x());
	emit heightChanged(dimension.y());
	emit dimensionChanged();
}

/**
 * @brief Returns the current dimension of the game
 * @return The current dimension
 */
QPoint Settings::getDimension() const
{
	return dimension;
}

/**
 * @brief Sets the width only of the game
 * @param width The new width
 */
void Settings::setWidth(int width)
{
	dimension.setX(width);
	setDimension(dimension);
}

/**
 * @brief Returns the width of the game
 * @return The width
 */
int Settings::getWidth() const
{
	return dimension.x();
}

/**
 * @brief Sets the height only of the game
 * @param height The new height
 */
void Settings::setHeight(int height)
{
	dimension.setY(height);
	setDimension(dimension);
}

/**
 * @brief Returns the height of the game
 * @return The height of the game
 */
int Settings::getHeight() const
{
	return dimension.y();
}

/**
 * @brief Sets the timeout waiting for the new round after the last one was finished
 * @param roundTimeOut The new timeout
 */
void Settings::setRoundTimeOut(int roundTimeOut)
{
	this->roundTimeOut = roundTimeOut;
}

/**
 * @brief Returns the current round timeout
 * @return The round timeout
 */
int Settings::getRoundTimeOut() const
{
	return roundTimeOut;
}

/**
 * @brief Sets the minimum amount of time that item spawns are apart
 * @param interval The new minimum amount of time
 */
void Settings::setItemSpawnIntervalMin(const int interval)
{
	this->itemSpawnIntervalMin = interval;
}

/**
 * @brief Returns the current minimum amount of time that item spawns are apart
 * @return Settings::itemSpawnIntervalMin
 */
int Settings::getItemSpawnIntervalMin() const
{
	return itemSpawnIntervalMin;
}

/**
 * @brief Sets the maximum amount of time that item spawns are apart
 * @param interval The new maximum amount of time
 */
void Settings::setItemSpawnIntervalMax(const int interval)
{
	this->itemSpawnIntervalMax = interval;
}

/**
 * @brief Returns hte current maximum amount of time that item spawns are apart
 * @return Settings::itemSpawnIntervalMax
 */
int Settings::getItemSpawnIntervalMax() const
{
	return itemSpawnIntervalMax;
}

/**
 * @brief Sets the username of the client
 * @param name The new username
 */
void Settings::setClientName(const QString name)
{
	clientName = name;
}

/**
 * @brief Returns the username of the client
 * @return Settings::clientName
 */
QString Settings::getClientName() const
{
	return clientName;
}

/**
 * @brief Sets the color of the client
 * @param color The new color
 */
void Settings::setClientColor(const QColor color)
{
	clientColor = color;
}

/**
 * @brief Returns the color of the client
 * @return Settings::clientColor
 */
QColor Settings::getClientColor() const
{
	return clientColor;
}

/**
 * @brief Sets the amount of points that have to be scored to win the game
 * @param score The new target score
 */
void Settings::setTargetScore(const int score)
{
	targetScore = score;
}

/**
 * @brief Returns the current target score
 * @return Settings::targetScore
 */
int Settings::getTargetScore() const
{
	return targetScore;
}

/**
 * @brief Sets the network blocking value
 * @param val The new value
 */
void Settings::setNetworkCurverBlock(const unsigned val)
{
	networkCurverBlock = val;
}

/**
 * @brief Returns the network blocking value
 * @return Settings::networkCurverBlock
 */
unsigned Settings::getNetworkCurverBlock() const
{
	return networkCurverBlock;
}

/**
 * @brief Sets the amount of logic updates per second
 * @param val The new value
 */
void Settings::setUpdatesPerSecond(const unsigned val)
{
	updatesPerSecond = val;
}

/**
 * @brief Returns the amount of logic updates per second
 */
unsigned Settings::getUpdatesPerSecond() const
{
	return updatesPerSecond;
}

/**
 * @brief Returns whether the application is started headless
 * @return Whether the application is started offscreen
 */
bool Settings::getOffscreen() const
{
	return QGuiApplication::platformName() == "offscreen";
}
