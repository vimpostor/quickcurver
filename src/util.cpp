#include "util.h"

void Util::init()
{
	qsrand(static_cast<uint>(QDateTime::currentMSecsSinceEpoch()));
}

double Util::rand()
{
	return static_cast<double>(qrand()) / RAND_MAX;
}

QPointF Util::randQPointF()
{
	return QPointF(rand(), rand());
}

int Util::randInt(const int lower, const int upper)
{
	return lower + rand() * (upper - lower);
}

QColor Util::randColor()
{
	auto it = colors.begin();
	std::advance(it, randInt(0, static_cast<int>(colors.size()) - 1));
	return it->second;
}

const QColor Util::getColor(const QString color)
{
	return colors.find(color)->second;
}

QString Util::expandIconName(const QString icon)
{
	return QString(":/Fluid/Controls/" + icon + ".svg");
}

bool Util::getBit(const uint8_t byte, const int pos)
{
	return (byte >> pos) % 2;
}
