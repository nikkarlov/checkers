#pragma once
#include "graphicsGame.h"
class Field {
 public:
	std::vector<std::vector<int>> vec;
	std::vector<std::vector<int>> activeField;
	std::vector<std::vector<int>> colorField;
};
Field field;
bool bot_fl = 0, isContinue = 0, moving = 0, gameContinue = 1;
int bot_level = 0, bot_col = 2, move_col = 1, countEatSteps = 0, indMenu = 0, winner = -1;
std::pair<int, int> prevButton{ -1,-1 }, pressedButton{ -1,-1 };
std::vector<std::pair<int, int>> simpleSteps;
void StartColorF() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0) {
				field.colorField[i][j] = 0;
			}
			else {
				field.colorField[i][j] = 1;
			}
		}
	}
}
void StartF() {
	field.vec.assign(8, std::vector<int>(8, 0));
	field.activeField.assign(8, std::vector<int>(8, 1));
	field.colorField.assign(8, std::vector<int>(8, 0));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (i < 3 && (i + j) % 2) {
				field.vec[i][j] = bot_col;
			}
			if (i > 4 && (i + j) % 2) {
				field.vec[i][j] = 3 - bot_col;
			}
		}
	}
	StartColorF();
	SetPosition(field.vec, field.colorField);
	PrintMenu(indMenu);
	window.display();
}
int GetPrevButtonColor(int x, int y) {
	if ((y + x) % 2  == 0) {
		return 0;
	}
	return 1;
}
void DeactivateField() {
	field.activeField.assign(8, std::vector<int>(8, 0));
}
void ActivateField() {
	field.activeField.assign(8, std::vector<int>(8, 1));
}
bool InsideField(int i, int j) {
	if (i < 8 && j < 8 && i >= 0 && j >= 0) {
		return 1;
	}
	return 0;
}

bool ÑellHasEatStep(int y, int x, bool oneStep, std::vector<int> dir) {
	bool eatStep = 0;
	int j = x + 1;
	for (int i = y - 1; i >= 0; i--) {
		if (dir[0] == 1 && dir[1] == -1 && !oneStep) {
			break;
		}
		if (InsideField(i, j)) {
			if (InsideField(i - 1, j + 1) && abs(field.vec[i - 1][j + 1]) == move_col) {
				break;
			}
			if (field.vec[i][j] != 0 && abs(field.vec[i][j]) != move_col) {
				eatStep = 1;
				if (!InsideField(i - 1, j + 1)) {
					eatStep = 0;
				}
				else if (field.vec[i - 1][j + 1] != 0) {
					eatStep = 0;
					break;
				}
				else {
					return eatStep;
				}
			}
		}
		if (j < 7) {
			j++;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
	}
	j = x - 1;
	for (int i = y - 1; i >= 0; i--) {
		if (dir[0] == 1 && dir[1] == 1 && !oneStep) {
			break;
		}
		if (InsideField(i, j)) {
			if (InsideField(i - 1, j - 1) && abs(field.vec[i - 1][j - 1]) == move_col) {
				break;
			}
			if (field.vec[i][j] != 0 && abs(field.vec[i][j]) != move_col) {
				eatStep = 1;
				if (!InsideField(i - 1, j - 1)) {
					eatStep = 0;
				}
				else if (field.vec[i - 1][j - 1] != 0) {
					eatStep = 0;
					break;
				}
				else {
					return eatStep;
				}
			}
		}
		if (j > 0) {
			j--;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
	}

	j = x - 1;
	for (int i = y + 1; i < 8; i++) {
		if (dir[0] == -1 && dir[1] == 1 && !oneStep) {
			break;
		}
		if (InsideField(i, j)) {
			if (InsideField(i + 1, j - 1) && abs(field.vec[i + 1][j - 1]) == move_col) {
				break;
			}
			if (field.vec[i][j] != 0 && abs(field.vec[i][j]) != move_col) {
				eatStep = 1;
				if (!InsideField(i + 1, j - 1)) {
					eatStep = 0;
				}
				else if (field.vec[i + 1][j - 1] != 0) {
					eatStep = 0;
					break;
				}
				else {
					return eatStep;
				}
			}
		}
		if (j > 0) {
			j--;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
	}
	j = x + 1;
	for (int i = y + 1; i < 8; i++) {
		if (dir[0] == -1 && dir[1] == -1 && !oneStep) {
			break;
		}
		if (InsideField(i, j)) {
			if (InsideField(i + 1, j + 1) && abs(field.vec[i + 1][j + 1]) == move_col) {
				break;
			}
			if (field.vec[i][j] != 0 && abs(field.vec[i][j]) != move_col) {
				eatStep = 1;
				if (!InsideField(i + 1, j + 1)) {
					eatStep = 0;
				}
				else if (field.vec[i + 1][j + 1] != 0) {
					eatStep = 0;
					break;
				}
				else {
					return eatStep;
				}
			}
		}
		if (j < 7) {
			j++;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
	}
	return eatStep;
}
void CloseSimpleSteps(std::vector<std::pair<int, int>> simpleSteps) {
	if (simpleSteps.size() > 0) {
		for (int i = 0; i < simpleSteps.size(); i++) {
			field.colorField[simpleSteps[i].first][simpleSteps[i].second] = GetPrevButtonColor(simpleSteps[i].first, simpleSteps[i].second);
			field.activeField[simpleSteps[i].first][simpleSteps[i].second] = 0;
		}
	}
}
void Eat(int y, int x, bool oneStep, bool fl_check) {
	int dirX = x - pressedButton.second;
	int dirY = y - pressedButton.first;
	if(dirX < 0){
		dirX = -1;
	}
	else {
		dirX = 1;
	}
	if (dirY < 0) {
		dirY = -1;
	}
	else {
		dirY = 1;
	}
	int il = y;
	int jl = x;
	bool isEmpty = 1;
	while (InsideField(il, jl)) {
		if (field.vec[il][jl] != 0 && abs(field.vec[il][jl]) != move_col) {
			isEmpty = 0;
			break;
		}
		il += dirY;
		jl += dirX;

		if (oneStep) {
			break;
		}
	}
	if (isEmpty) {
		return;
	}
	std::vector<std::pair<int, int>> toClose;
	bool closeSimple = 0;
	int ik = il + dirY;
	int jk = jl + dirX;
	while (InsideField(ik, jk)) {
		if (field.vec[ik][jk] == 0) {
			if (ÑellHasEatStep(ik, jk, oneStep, { dirX, dirY })) {
				closeSimple = true;
			}
			else {
				toClose.push_back(std::make_pair(ik, jk));
			}
			if (!fl_check) {
				field.colorField[ik][jk] = 3;
				field.activeField[ik][jk] = 1;
			}
			countEatSteps++;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
		jk += dirX;
		ik += dirY;
	}
	if (closeSimple && toClose.size() > 0) {
		CloseSimpleSteps(toClose);
	}
}
bool DeterminePath(int y, int x, bool fl_eat, bool fl_check) {
	if (field.vec[y][x] == 0 && !isContinue && !fl_eat) {
		if (!fl_check) {
			field.colorField[y][x] = 3;
			field.activeField[y][x] = 1;
		}
		simpleSteps.push_back(std::make_pair(y, x));
	}
	else {
		if (abs(field.vec[y][x]) != move_col) {
			if (field.vec[pressedButton.first][pressedButton.second] < 0) {
				Eat(y, x, 0, fl_check);
			}
			else {
				Eat(y, x, 1, fl_check);
			}
		}
		return 0;
	}
	return 1;
}
void Diagonal(int y, int x, bool oneStep, bool fl_check) {
	bool fl_eat;
	int j = x + 1;
	if ((move_col == bot_col) && oneStep && !isContinue) {
		fl_eat = 1;
	}
	else {
		fl_eat = 0;
	}
	for (int i = y - 1; i >= 0; i--) {
		if (InsideField(i, j)) {
			if (!DeterminePath(i, j, fl_eat, fl_check)) {
				break;
			}
		}
		if (j < 7) {
			j++;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
	}
	j = x - 1;
	for (int i = y - 1; i >= 0; i--) {
		if (InsideField(i, j)) {
			if (!DeterminePath(i, j, fl_eat, fl_check)) {
				break;
			}
		}
		if (j > 0) {
			j--;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
	}
	if ((move_col != bot_col) && oneStep && !isContinue) {
		fl_eat = 1;
	}
	else {
		fl_eat = 0;
	}
	j = x - 1;
	for (int i = y + 1; i < 8; i++) {
		if (InsideField(i, j)){
			if (!DeterminePath(i, j, fl_eat, fl_check)) {
				break;
			}
		}
		if (j > 0) {
			j--;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
	}
	j = x + 1;
	for (int i = y + 1; i < 8; i++) {
		if (InsideField(i, j)) {
			if (!DeterminePath(i, j, fl_eat, fl_check)) {
				break;
			}
		}
		if (j < 7) {
			j++;
		}
		else {
			break;
		}
		if (oneStep) {
			break;
		}
	}
}
void PossibleSteps() {
	bool isOneStep = true;
	bool isEatStep = false;
	DeactivateField();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (abs(field.vec[i][j]) == move_col) {
				if (field.vec[i][j] < 0) {
					isOneStep = 0;
				}
				else {
					isOneStep = 1;
				}
				if (ÑellHasEatStep(i, j, isOneStep, { 0, 0 })) {
					isEatStep = 1;
					field.activeField[i][j] = 1;
				}
			}
		}
	}
	if (!isEatStep)
		ActivateField();
}
void Steps(int y, int x, bool isOnestep = 1, bool fl_check = 0) {
	simpleSteps.clear();
	Diagonal(y, x, isOnestep, fl_check);
	if (countEatSteps > 0) {
		CloseSimpleSteps(simpleSteps);
	}
}
void DeleteEaten(std::pair<int, int> endButton, std::pair<int, int> startButton) {
	int count = abs(endButton.first - startButton.first);
	int startIndexX = endButton.first - startButton.first;
	int startIndexY = endButton.second - startButton.second;
	startIndexX = startIndexX < 0 ? -1 : 1;
	startIndexY = startIndexY < 0 ? -1 : 1;
	int currCount = 0;
	int i = startButton.first + startIndexX;
	int j = startButton.second + startIndexY;
	while (currCount < count - 1)
	{
		field.vec[i][j] = 0;
		i += startIndexX;
		j += startIndexY;
		currCount++;
	}
}
void SwitchButtonToCheat(std::pair<int, int> button) {
	if (field.vec[button.first][button.second] == bot_col && button.first == 7){
		field.vec[button.first][button.second] = -abs(field.vec[button.first][button.second]);
	}
	if (field.vec[button.first][button.second] != bot_col && button.first == 0)
	{
		field.vec[button.first][button.second] = -abs(field.vec[button.first][button.second]);
	}
}
void CheckEndGame() {
	bool player1 = 0, player2 = 0, isContinue1 = isContinue;
	isContinue = 0;
	int move_col1 = move_col;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			countEatSteps = 0;
			if (field.vec[i][j] != 0) {
				move_col = abs(field.vec[i][j]);
				if (field.vec[i][j] < 0) {
					Steps(i, j, 0, 1);
				}
				else {
					Steps(i, j, 1, 1);
				}
				if (countEatSteps || simpleSteps.size()) {
					if (abs(field.vec[i][j]) == 1) {
						player1 = 1;
					}
					else {
						player2 = 1;
					}
				}
			}
		}
	}
	move_col = move_col1, isContinue = isContinue1;
	if (!player1 || !player2) {
		gameContinue = 0;
		if (!player1) {
			winner = 2;
		}
		else {
			winner = 1;
		}
	}
}
void ÑhangeMove() {
	move_col = 3 - move_col;
	CheckEndGame();
}
void CellPress() {
	if (prevButton.first != -1) {
		int prevButtonColor = field.vec[prevButton.first][prevButton.second];
	}
	if (abs(field.vec[pressedButton.first][pressedButton.second]) == move_col) {
		StartColorF();
		field.colorField[pressedButton.first][pressedButton.second] = 2;
		DeactivateField();
		field.activeField[pressedButton.first][pressedButton.second] = 1;
		countEatSteps = 0;
		if (field.vec[pressedButton.first][pressedButton.second] < 0) {
			Steps(pressedButton.first, pressedButton.second, 0);
		}
		else {
			Steps(pressedButton.first, pressedButton.second);
		}
		if (moving) {
			StartColorF();
			field.colorField[pressedButton.first][pressedButton.second] = GetPrevButtonColor(pressedButton.first, pressedButton.second);
			PossibleSteps();
			moving = 0;
		}
		else
			moving = 1;
	}
	else {
		if (moving) {
			isContinue = 0;
			if (countEatSteps > 0) {
				isContinue = 1;
				DeleteEaten(pressedButton, prevButton);
			}
			int temp = abs(field.vec[pressedButton.first][pressedButton.second]);
			field.vec[pressedButton.first][pressedButton.second] = field.vec[prevButton.first][prevButton.second];
			field.vec[prevButton.first][prevButton.second] = temp;
			SwitchButtonToCheat(pressedButton);
			countEatSteps = 0;
			moving = 0;
			StartColorF();
			DeactivateField();
			if (field.vec[pressedButton.first][pressedButton.second] < 0) {
				Steps(pressedButton.first, pressedButton.second, 0);
			}
			else {
				Steps(pressedButton.first, pressedButton.second);
			}
			if (countEatSteps == 0 || !isContinue) {
				StartColorF();
				ÑhangeMove();
				PossibleSteps();
				isContinue = 0;
			}
			else if (isContinue) {
				field.colorField[pressedButton.first][pressedButton.second] = 2;
				field.activeField[pressedButton.first][pressedButton.second] = 1;
				moving = 1;
			}
		}
	}
	prevButton = pressedButton;
}
void ClickHandlingF(int x, int y) {
	y -= heightIm, x -= widhtIm;
	pressedButton.first = y / cellSize, pressedButton.second = x / cellSize;
	if (field.activeField[pressedButton.first][pressedButton.second] && gameContinue && (move_col != bot_col || !bot_fl)) {
		CellPress();
	}
}
void ClickHandlingB(int x, int y) {
	for (int i = 0; i < 5; i++) {
		if (x >= menu[i].x_s && x <= menu[i].x_e && y >= menu[i].y_s && y <= menu[i].y_e) {
			isContinue = 0, moving = 0, gameContinue = 1;
			prevButton = { -1, -1 }, pressedButton = { -1,-1 }, move_col = 1, winner = -1;
			simpleSteps.clear();
			switch (i)
			{
			case 0:
				bot_fl = 0, indMenu = 0; break;
			case 1:
				bot_fl = 1, bot_level = 1, indMenu = 1; break;
			case 2:
				bot_fl = 1, bot_level = 2, indMenu = 2; break;
			case 3:
				bot_fl = 1, bot_level = 3, indMenu = 3; break;
			case 4:
				bot_col = 3 - bot_col; break;
			default:
				break;
			}
			StartF();
		}
	}
}
void ClickHandling(Event event) {
	if (event.type == sf::Event::Closed) {
		window.close();
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			int x = event.mouseButton.x;
			int y = event.mouseButton.y;
			if (x >= widhtIm && x <= widhtIm + 800 && y >= heightIm && y <= heightIm + 800) {
				ClickHandlingF(x, y);
			}
			else {
				if (x >= x_s && x <= x_s + 200 && y >= y_s && y <= y_s + 500) {
					ClickHandlingB(x, y);
				}
			}
		}
	}
}