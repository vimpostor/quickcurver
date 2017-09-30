#ifndef SETTINGS_H
#define SETTINGS_H

#include <QPoint>
#include <QObject>
#include <QColor>

class Settings : public QObject
{
	Q_OBJECT
public:
	explicit Settings();
	static Settings &getSingleton();
	void setDimension(QPoint dimension);
	QPoint getDimension() const;
	Q_INVOKABLE void setWidth(int width);
	Q_INVOKABLE void setHeight(int height);
	Q_INVOKABLE void setRoundTimeOut(int roundTimeOut);
	Q_INVOKABLE int getRoundTimeOut() const;
	Q_INVOKABLE void setItemSpawnIntervalMin(const int interval);
	Q_INVOKABLE int getItemSpawnIntervalMin() const;
	Q_INVOKABLE void setItemSpawnIntervalMax(const int interval);
	Q_INVOKABLE int getItemSpawnIntervalMax() const;
	Q_INVOKABLE void setClientName(const QString name);
	QString getClientName() const;
	Q_INVOKABLE void setClientColor(const QColor color);
	QColor getClientColor() const;
signals:
	void dimensionChanged();
private:
	QString clientName;
	QColor clientColor;
	QPoint dimension {100, 100};
	int roundTimeOut = 1000;
	int itemSpawnIntervalMin = 1000;
	int itemSpawnIntervalMax = 5000;
};

#endif // SETTINGS_H
