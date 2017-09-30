#ifndef BOT_H
#define BOT_H

#include "models/playermodel.h"
#include "settings.h"

class Bot
{
public:
	Bot() = delete;
	static void makeMove(Curver *c);
};

#endif // BOT_H
