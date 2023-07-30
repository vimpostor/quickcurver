#pragma once

#include <QColor>
#include <QGuiApplication>
#include <QObject>
#include <QPoint>

/**
 * @brief This class represents settings that affect the game in any way
 */
class Settings : public QObject {
	Q_OBJECT

	Q_PROPERTY(int width READ getWidth WRITE setWidth NOTIFY widthChanged)
	Q_PROPERTY(int height READ getHeight WRITE setHeight NOTIFY heightChanged)
public:
	explicit Settings();
	static Settings &getSingleton();
	void setDimension(QPoint dimension);
	QPoint getDimension() const;
	Q_INVOKABLE void setWidth(int width);
	Q_INVOKABLE int getWidth() const;
	Q_INVOKABLE void setHeight(int height);
	Q_INVOKABLE int getHeight() const;
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
	Q_INVOKABLE void setTargetScore(const int score);
	Q_INVOKABLE int getTargetScore() const;
	Q_INVOKABLE void setNetworkCurverBlock(const unsigned val);
	Q_INVOKABLE unsigned getNetworkCurverBlock() const;
	Q_INVOKABLE void setUpdatesPerSecond(const unsigned val);
	Q_INVOKABLE unsigned getUpdatesPerSecond() const;
	Q_INVOKABLE bool getOffscreen() const;
signals:
	/**
	 * @brief Emitted, when the dimension of the game changed
	 */
	void dimensionChanged();
	/**
	 * @brief Emitted, when the width changed
	 * @param width The new width
	 */
	void widthChanged(int width);
	/**
	 * @brief Emitted, when the height changed
	 * @param height The new height
	 */
	void heightChanged(int height);
private:
	/**
	 * @brief The username of the client
	 */
	QString clientName;
	/**
	 * @brief The color of the client
	 */
	QColor clientColor;
	/**
	 * @brief The dimension of the game
	 */
	QPoint dimension {700, 836};
	/**
	 * @brief The current round time out
	 *
	 * This is the amount of time that has to be waited for the next round, after the old one was finished
	 */
	int roundTimeOut = 1000;
	/**
	 * @brief The minimum amount of time between two item spawns
	 */
	int itemSpawnIntervalMin = 1000;
	/**
	 * @brief The maximum amount of time between two item spawns
	 */
	int itemSpawnIntervalMax = 5000;
	/**
	 * @brief The score to achieve to win the game
	 */
	int targetScore = 15;
	/**
	 * @brief Determines how often Curver data should be sent by the Server
	 *
	 * A value of n means, that n-1 times no data will be sent before data will be sent again.
	 * A value of 1 means, that every iteration all data will be sent.
	 */
	unsigned networkCurverBlock = 2;
	/**
	 * @brief The number of logic updates per second
	 */
	unsigned updatesPerSecond = 60;
};
