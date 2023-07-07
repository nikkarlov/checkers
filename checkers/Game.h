#pragma once
#include "bot.h"
void botMove() {
	if (bot_level == 1) {
		EasyBot bot;
		bot.Move();
	}
	else if (bot_level == 2) {
		NormalBot bot;
		bot.Move();
	}
	else {
		HardBot bot;
		bot.Move();
	}
}
void StartGame() {
	Cr_but();
	StartF();
	while (window.isOpen()) {
		Event event;
		if (gameContinue && bot_fl && bot_col == move_col) {
			botMove();
		}
		while (window.pollEvent(event)) {
			ClickHandling(event);
			SetPosition(field.vec, field.colorField);
			PrintMenu(indMenu);
			if (!gameContinue) {
				PrintWin(winner);
			}
			window.display();
			if (gameContinue && bot_fl && bot_col == move_col) {
				botMove();
			}
		}
	}
}