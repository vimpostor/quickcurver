#pragma once

#include "models/playermodel.hpp"
#include "settings.hpp"

/**
 * @brief A class representing an AI that controls a Curver
 */
class Bot {
public:
	Bot() = delete;
	static void makeMove(Curver &c);
};
