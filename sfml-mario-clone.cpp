#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <conio.h>
#include "Structures.h"

#define SCREENSHOT 115
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define TOTAL_WIDTH 6688.f

using namespace std;
using namespace sf;
using namespace std::this_thread;
using namespace std::chrono_literals;
using std::chrono::system_clock;

char** lvlArr;
vector<platform> vp;
vector<enemy*> ve;

void getLevel()
{
	lvlArr = new char* [15];
	for (int i = 0; i < 15; ++i)
		lvlArr[i] = new char[209];

	fstream lvl;
	lvl.open("Level.txt", ios::in);
	if (!lvl.good())
	{
		cout << "file error";
		throw 1;
	}
	char c = 0;

	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 209; ++j)
		{
			do
			{
				c = lvl.get();
			} while (c == '\n' || c == '\r');

			lvlArr[i][j] = c;
		}
	}
	lvl.close();
}

void screenShot(char** arr)
{
	fstream file;
	file.open("screenshot.txt", ios::out);
	if (!file.good())
	{
		cout << "file error";
		throw 1;
	}
	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 209; ++j)
		{
			file << arr[i][j];
		}
		file << endl;
	}
	file.close();
}

int main()
{
	getLevel();

	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mari...cough...Italian plumber");

	Texture playerImage;
	playerImage.loadFromFile("mario.png");
	player player(playerImage);

	Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "Unnable to load font " << endl;
		return 1;
	}

	Text coins;
	coins.setFont(font);
	coins.setString(to_string(player.coins));
	coins.setCharacterSize(16);
	coins.setFillColor(Color::White);

	Texture blockSprite;
	blockSprite.loadFromFile("platform.png");

	Texture tul;
	tul.loadFromFile("tul.png");

	Texture tur;
	tur.loadFromFile("tur.png");

	Texture tdl;
	tdl.loadFromFile("tdl.png");

	Texture tdr;
	tdr.loadFromFile("tdr.png");

	Texture qSprite;
	qSprite.loadFromFile("q.png");

	Texture enemySprite;
	enemySprite.loadFromFile("goomba.png");

	enemy en1(enemySprite, 390.f, 398.f);
	enemy en2(enemySprite, 966.f, 398.f);
	enemy en3(enemySprite, 1350.f, 398.f);
	enemy en4(enemySprite, 1382.f, 398.f);
	enemy en5(enemySprite, 2278.f, 192.f);
	enemy en6(enemySprite, 2310.f, 192.f);
	enemy en7(enemySprite, 2822.f, 398.f);
	enemy en8(enemySprite, 2854.f, 398.f);
	enemy en9(enemySprite, 3366.f, 398.f);
	enemy en10(enemySprite, 3398.f, 398.f);
	enemy en11(enemySprite, 3686.f, 398.f);
	enemy en12(enemySprite, 3718.f, 398.f);
	enemy en13(enemySprite, 3814.f, 398.f);
	enemy en14(enemySprite, 3846.f, 398.f);
	enemy en15(enemySprite, 5286.f, 398.f);
	enemy en16(enemySprite, 5318.f, 398.f);

	ve.push_back(&en1);
	ve.push_back(&en2);
	ve.push_back(&en3);
	ve.push_back(&en4);
	ve.push_back(&en5);
	ve.push_back(&en6);
	ve.push_back(&en7);
	ve.push_back(&en8);
	ve.push_back(&en9);
	ve.push_back(&en10);
	ve.push_back(&en11);
	ve.push_back(&en12);
	ve.push_back(&en13);
	ve.push_back(&en14);
	ve.push_back(&en15);

	float x1, y1, tx = -WINDOW_WIDTH / 2;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case Keyboard::Left:
					player.left = true;
					break;
				case Keyboard::Up:
					player.up = true;
					break;
				case Keyboard::Right:
					player.right = true;
					break;
				case Keyboard::Escape:
					window.close();
				}
			}
			if (event.type == Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case Keyboard::Left:
					player.left = false;
					break;
				case Keyboard::Up:
					player.up = false;
					break;
				case Keyboard::Right:
					player.right = false;
					break;
				}
			}
			if (event.type == Event::Closed)
				window.close();
		}

		auto diff = WINDOW_WIDTH / 2 - player.rect.left;
		if (diff < 0) {
			tx += diff;
			player.rect.left = WINDOW_WIDTH / 2;
			for (enemy* en : ve)
			{
				en->rect.left += diff;
			}
		}
		for (int i = 0; i < 15; ++i)
		{
			y1 = i * BLOCK_SIZE;
			for (int j = 0; j < 209; ++j)
			{
				x1 = j * BLOCK_SIZE + tx;
				if (lvlArr[i][j] == '#')
				{
					platform pl(blockSprite, x1, y1, '#');
					vp.push_back(pl);
				}
				else if (lvlArr[i][j] == '?')
				{
					platform pl(qSprite, x1, y1, '?');
					vp.push_back(pl);
				}
				else if (lvlArr[i][j] == '-')
				{
					platform pl(tul, x1, y1, '-');
					vp.push_back(pl);
				}
				else if (lvlArr[i][j] == '_')
				{
					platform pl(tur, x1, y1, '_');
					vp.push_back(pl);
				}
				else if (lvlArr[i][j] == '|')
				{
					platform pl(tdl, x1, y1, '|');
					vp.push_back(pl);
				}
				else if (lvlArr[i][j] == '/')
				{
					platform pl(tdr, x1, y1, '/');
					vp.push_back(pl);
				}
			}
		}

		window.clear(Color(107, 140, 255));

		for (enemy* en : ve)
		{
			if (en->alive && en->rect.left > 0 - ENEMY_WIDTH && en->rect.left < WINDOW_WIDTH)
			{
				en->update(vp);
				window.draw(en->sprite);
			}
		}

		if (player.update(vp, ve))
			window.close();
		coins.setString(to_string(player.coins));

		for (platform pl : vp)
		{
			window.draw(pl.sprite);
		}
		window.draw(player.sprite);
		window.draw(coins);

		vp.clear();
		window.display();
		sleep_for(5ms);
	}
}