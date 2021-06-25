#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#define BLOCK_SIZE 32.f
#define ENEMY_WIDTH 18.f
#define ENEMY_HEIGHT 18.f
#define ENEMY_SPEED 0.5f;
#define PLAYER_WIDTH 20.f
#define PLAYER_HEIGHT 34.f
#define SPEED 3.f
#define GRAVITY 0.35f
#define JUMP 9.5f

using namespace std;
using namespace sf;

class platform
{
public:
	FloatRect rect;
	Sprite sprite;
	char block;
	platform(Texture&, float, float, char);
};

class enemy
{
public:
	float vx, vy, sh = 0.f;
	int i;
	FloatRect rect;
	bool onGround = false, alive = true, hided = false;
	Sprite sprite;
	enemy(Texture&, float, float);
	void update(vector<platform>);
	void collide(int, vector<platform>);
};

class player
{
public:
	float vx, vy;
	FloatRect rect;
	bool onGround = false, up = false, left = false, right = false, alive = true;
	int coins = 0;
	Sprite sprite;
	player(Texture&);
	int update(vector<platform>, vector<enemy*>);
	void collide(float, float, vector<platform>);
};