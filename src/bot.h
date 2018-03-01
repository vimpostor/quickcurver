#ifndef BOT_H
#define BOT_H

#include "models/playermodel.h"
#include "settings.h"

/**
 * @brief A class representing an AI that controls a Curver
 */
class Bot
{
public:
	Bot() = delete;
	static void makeMove(Curver *c);
};

#endif // BOT_H
