#pragma once
#include "logicGame.h"
#include <chrono>
#include <ctime>
#include <random>
std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
class Bot {
 public:
	virtual void Move() = 0;
	void getMove(std::vector<std::pair<int, int>>& move, int who_col) {
		move.clear();
		bool fl_steps = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (!field.activeField[i][j]) {
					fl_steps = 1;
				}
				else {
					if (pressedButton.first != i || pressedButton.second != j) {
						move.push_back({ i, j });
					}
				}
			}
		}
		if (!fl_steps) {
			move.clear();
			bool isContinue1 = isContinue;
			isContinue = 0;
			int move_col1 = move_col;
			std::pair<int, int> pressedButton1 = pressedButton;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					countEatSteps = 0;
					pressedButton.first = i, pressedButton.second = j;
					if (field.vec[i][j] != 0) {
						move_col = abs(field.vec[i][j]);
						if (field.vec[i][j] < 0) {
							Steps(i, j, 0, 1);
						}
						else {
							Steps(i, j, 1, 1);
						}
						if (countEatSteps || simpleSteps.size()) {
							if (abs(field.vec[i][j]) == who_col) {
								move.push_back({ i, j });
							}
						}
					}
				}
			}
			pressedButton = pressedButton1;
			move_col = move_col1, isContinue = isContinue1;
		}
	}
};
class EasyBot : public Bot {
 public:
	 void Move() {
		 int move_col1 = move_col;
		 while (move_col1 == move_col) {
			 std::vector<std::pair<int, int>> move2;
			 getMove(move2, bot_col);
			 int randMove = rnd() % move2.size();
			 pressedButton.first = move2[randMove].first, pressedButton.second = move2[randMove].second;
			 CellPress();
		 }
	 }
};
struct AnsDfs {
	std::pair<std::pair<int, int>, std::pair<int, int>> move;
	int heuristic;
};
class LogicalBot : public Bot {
 public:
	int heuristic(int depth) {
		int k = 3;
		int playerCount = 0, botCount = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (field.vec[i][j] != 0) {
					if (abs(field.vec[i][j]) == bot_col) {
						if (field.vec[i][j] < 0) {
							botCount += k;
						}
						else {
							botCount++;
						}
					}
					else {
						if (field.vec[i][j] < 0) {
							playerCount += k;
						}
						else {
							playerCount++;
						}
					}
				}
			}
		}
		return botCount - playerCount - depth;
	}
	AnsDfs dfs(Field vec, int depth) {
		AnsDfs ansDfs;
		field = vec;
		std::vector<std::pair<int, int>> move;
		std::vector<std::pair<int, int>> move1;
		if (depth == getmaxDepth()) {
			ansDfs.heuristic = heuristic(depth);
			return ansDfs;
		}
		getMove(move, move_col);
		if (depth % 2 == 0) {
			ansDfs.heuristic = -1e4;
		}
		else {
			ansDfs.heuristic = 1e4;
		}
		for (int i = 0; i < move.size(); i++) {
			field = vec;
			int move_col1 = move_col, winner1 = winner;
			bool isContinue1 = isContinue, moving1 = moving, gameContinue1 = gameContinue;
			pressedButton.first = move[i].first, pressedButton.second = move[i].second;
			CellPress();
			getMove(move1, move_col);
			Field field1 = field;
			std::pair<int, int> prevButton1 = pressedButton;
			for (int j = 0; j < move1.size(); j++) {
				field = field1;
				prevButton = prevButton1;
				int move_col2 = move_col, winner2 = winner;
				bool isContinue2 = isContinue, moving2 = moving, gameContinue2 = gameContinue;
				pressedButton.first = move1[j].first, pressedButton.second = move1[j].second;
				CellPress();
				while (move_col1 == move_col) {
					std::vector<std::pair<int, int>> move2;
					getMove(move2, move_col);
					pressedButton.first = move2[0].first, pressedButton.second = move2[0].second;
					CellPress();
				}
				AnsDfs temporaryAns = dfs(field, depth + 1);
				if (depth % 2 == 0) {
					if (temporaryAns.heuristic > ansDfs.heuristic) {
						ansDfs.heuristic = temporaryAns.heuristic;
						ansDfs.move.first = move[i];
						ansDfs.move.second = move1[j];
					}
				}
				else {
					if (temporaryAns.heuristic < ansDfs.heuristic) {
						ansDfs.heuristic = temporaryAns.heuristic;
						ansDfs.move.first = move[i];
						ansDfs.move.second = move1[j];
					}
				}
				move_col = move_col2, winner = winner2;
				isContinue = isContinue2, moving = moving2, gameContinue = gameContinue2;
			}
			move_col = move_col1, winner = winner1;
			isContinue = isContinue1, moving = moving1, gameContinue = gameContinue1;
		}
		return ansDfs;
	}
	std::pair<std::pair<int, int>, std::pair<int, int>> RightMove() {
		int countMove = 0;
		int move_col1 = move_col, winner1 = winner;
		bool isContinue1 = isContinue, moving1 = moving, gameContinue1 = gameContinue;
		Field field1 = field;
		AnsDfs ansDfs = dfs(field, 0);
		field = field1;
		move_col = move_col1, winner = winner1;
		isContinue = isContinue1, moving1 = moving1, gameContinue1 = gameContinue1;
		return ansDfs.move;
	}
	void Move() {
		int move_col1 = move_col;
		std::pair<std::pair<int, int>, std::pair<int, int>> rightMove = RightMove();
		pressedButton.first = rightMove.first.first, pressedButton.second = rightMove.first.second;
		CellPress();
		pressedButton.first = rightMove.second.first, pressedButton.second = rightMove.second.second;
		CellPress();
		while (move_col1 == move_col) {
			std::vector<std::pair<int, int>> move2;
			getMove(move2, move_col);
			pressedButton.first = move2[0].first, pressedButton.second = move2[0].second;
			CellPress();
		}
	}
	virtual int getmaxDepth() = 0;
};
class NormalBot : public LogicalBot {
 public:
	 int getmaxDepth() {
		 return 2;
	 }
};
class HardBot : public LogicalBot {
 public:
	 int getmaxDepth() {
		 return 5;
	 }
};