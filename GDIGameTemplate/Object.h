#pragma once

struct Enemy
{
	float x, y;
	float speed;
	float dirX, dirY;
	bool bAlive;
};

struct Player
{
	float x, y;
	float speed;
	float dirX, dirY;

	void Update();
};

struct Bullet
{
	float x,y;
	float speed;
	float dirX, dirY;
	bool bPlayer;
	bool bAlive;
};

