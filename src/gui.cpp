#include "gui.h"

Gui::Gui(QObject *parent) : QObject(parent)
{
}

const Gui &Gui::getSingleton()
{
	static Gui result;
	return result;
}
