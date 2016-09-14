#include "playerselector.h"

playerSelector::playerSelector(Game *game, QObject *parent) : QObject(parent) {
	this->game = game;
}

void playerSelector::newPlayerSlot() {
	game->addPlayer();
	game->changeColor(game->playercount-1, "blue");
}

void playerSelector::colorChanged(int index, QColor color) {
	game->changeColor(index, color);
}

void playerSelector::controlsChanged(int index, Qt::Key key, bool isRight) {
	game->changeControls(index, key, isRight);
}
