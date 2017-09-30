#ifndef GUI_H
#define GUI_H

#include <QObject>

class Gui : public QObject
{
	Q_OBJECT
public:
	explicit Gui(QObject *parent = 0);
	static const Gui &getSingleton();
signals:
	void postInfoBar(QString msg) const;
	void startGame() const;
};

#endif // GUI_H
