#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
using namespace sf;
int widthScreen = VideoMode::getFullscreenModes().data()->width;
int heightScreen = VideoMode::getFullscreenModes().data()->height;
int x_s = widthScreen / 2 + 500, y_s = heightScreen / 2 - 200;
int cellSize = 100;
RenderWindow window(sf::VideoMode(widthScreen, heightScreen), "game");//, Style::Fullscreen);
int widhtIm = widthScreen / 2 - 400, heightIm = heightScreen / 2 - 400;
class Button {
 public:
	int x_s, x_e, y_s, y_e;
	std::string str_;
 private:
	 void Swap(Button other) {
		 std::swap(x_s, other.x_s);
		 std::swap(x_e, other.x_e);
		 std::swap(y_s, other.y_s);
		 std::swap(y_e, other.y_e);
		 std::swap(str_, other.str_);
	 }
};
std::vector<Button> menu;
void Cr_but() {
	Button button = { x_s,x_s + 105,y_s + 6,y_s + 27,"2 players" };
	menu.push_back(button);
	button = { x_s,x_s + 50,y_s + 110,y_s + 127,"easy" };
	menu.push_back(button);
	button = { x_s,x_s + 75,y_s + 210,y_s + 227,"normal" };
	menu.push_back(button);
	button = { x_s,x_s + 50,y_s + 310,y_s + 327,"hard" };
	menu.push_back(button);
	button = { x_s,x_s + 75,y_s + 410,y_s + 427,"reverse" };
	menu.push_back(button);
}
void SetField(std::vector<std::vector<int>> colorField) {
	Image image_table;
	image_table.loadFromFile("C:\\Users\\Nikolay\\source\\repos\\checkers\\x64\\Debug\\images\\table.png");
	Texture t_table;
	t_table.loadFromImage(image_table);
	Sprite s_table;
	s_table.setScale({ float(widthScreen / 1200.), float(heightScreen / 800.) });
	s_table.setTexture(t_table);
	window.clear();
	window.draw(s_table);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
			switch (colorField[i][j])
			{
			case 0: rectangle.setFillColor(Color(240, 202, 159)); break;
			case 1: rectangle.setFillColor(Color(178, 103, 17)); break;
			case 2: rectangle.setFillColor(Color::Red); break;
			case 3: rectangle.setFillColor(Color::Yellow); break;
			default:
				break;
			}
			rectangle.setPosition(j * cellSize + widhtIm, i * cellSize + heightIm);
			window.draw(rectangle);
		}
	}
}
void SetPosition(std::vector<std::vector<int>>& pos, std::vector<std::vector<int>> colorField) {
	SetField(colorField);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (abs(pos[i][j]) == 1 || abs(pos[i][j]) == 2) {
				CircleShape circle(50);
				if (abs(pos[i][j]) == 2) {
					circle.setFillColor(Color(0, 0, 0));
				}
				else {
					circle.setFillColor(Color(255, 255, 255));
				}
				circle.setPosition(j * cellSize + widhtIm, i * cellSize + heightIm);
				window.draw(circle);
				if (pos[i][j] < 0) {
					CircleShape circle1(10);
					circle1.setFillColor(Color(255, 0, 0));
					circle1.setPosition(j * cellSize + widhtIm + cellSize / 2 - 10, i * cellSize + heightIm + cellSize / 2 - 10);
					window.draw(circle1);
				}
			}
		}
	}
}
void PrintMenu(int ind) {
	Text text;
	Font font;
	font.loadFromFile("C:\\Users\\Nikolay\\source\\repos\\checkers\\x64\\Debug\\caviar-dreams.ttf");
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Red);
	for (int i = 0; i < 5; i++) {
		if (i == ind) {
			text.setFillColor(sf::Color::Green);
		}
		if(i == 4){
			text.setFillColor(sf::Color::Yellow);
		}
		text.setString(menu[i].str_);
		text.setPosition(x_s, y_s + i * 100);
		window.draw(text);
		if (i == ind) {
			text.setFillColor(sf::Color::Red);
		}
	}
}
void PrintWin(int winner) {
	Text text;
	Font font;
	font.loadFromFile("C:\\Users\\Nikolay\\source\\repos\\checkers\\x64\\Debug\\caviar-dreams.ttf");
	text.setFont(font);
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::Red);
	text.setPosition(widthScreen / 2 - 105, heightIm - 100);
	if (winner == 1) {
		text.setString("WHITE WIN");
	}
	else {
		text.setString("BLACK WIN");
	}
	window.draw(text);
}