#ifndef PLAYERSELECTOR_H
#define PLAYERSELECTOR_H
#include <QObject>
#include <QDebug>
#include "game.h"
#include <QtQuick>
#include <QtQuickControls2/QtQuickControls2>


class playerSelector : public QObject
{
	Q_OBJECT
public:
	explicit playerSelector(Game* game, QObject *parent = 0);

signals:

public slots:
	void newPlayerSlot();
	void colorChanged(int index, QColor color);
	void controlsChanged(int index, Qt::Key key, bool isRight);
private:
	Game* game;
};

#endif // PLAYERSELECTOR_H
