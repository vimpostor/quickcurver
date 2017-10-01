#include "settings.h"

Settings::Settings()
{
}

Settings &Settings::getSingleton()
{
	static Settings result;
	return result;
}

void Settings::setDimension(QPoint dimension)
{
	this->dimension = dimension;
	emit dimensionChanged();
}

QPoint Settings::getDimension() const
{
	return dimension;
}

void Settings::setWidth(int width)
{
	dimension.setX(width);
	setDimension(dimension);
}

void Settings::setHeight(int height)
{
	dimension.setY(height);
	setDimension(dimension);
}

void Settings::setRoundTimeOut(int roundTimeOut)
{
	this->roundTimeOut = roundTimeOut;
}

int Settings::getRoundTimeOut() const
{
	return roundTimeOut;
}

void Settings::setItemSpawnIntervalMin(const int interval)
{
	this->itemSpawnIntervalMin = interval;
}

int Settings::getItemSpawnIntervalMin() const
{
	return itemSpawnIntervalMin;
}

void Settings::setItemSpawnIntervalMax(const int interval)
{
	this->itemSpawnIntervalMax = interval;
}

int Settings::getItemSpawnIntervalMax() const
{
	return itemSpawnIntervalMax;
}

void Settings::setClientName(const QString name)
{
	clientName = name;
}

QString Settings::getClientName() const
{
	return clientName;
}

void Settings::setClientColor(const QColor color)
{
	clientColor = color;
}

QColor Settings::getClientColor() const
{
	return clientColor;
}

void Settings::setTargetScore(const int score)
{
	targetScore = score;
}

int Settings::getTargetScore() const
{
	return targetScore;
}
