#ifndef GUI_H
#define GUI_H

#include <QObject>

/**
 * @brief A centralized place for notifying the GUI about something
 */
class Gui : public QObject {
	Q_OBJECT
public:
	static const Gui &getSingleton();
signals:
	/**
	 * @brief Emitted, when someone wants to post the infobar
	 * @param msg The message to display
	 */
	void postInfoBar(QString msg) const;
	/**
	 * @brief Emitted, when the game started
	 */
	void startGame() const;
};

#endif // GUI_H
