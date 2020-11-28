#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace sf;

class GameEngine
{
public:
	const float movementSpeed;
	const float BulletSpeed;
	const int fireCap;
	//const int spawnCap = 20;

	GameEngine()
		: movementSpeed(1.5f),
		BulletSpeed(3.0f),
		fireCap(40)
	{}
};

class Bullet
{
public:
	CircleShape shape;  // shape contains position and size
	Vector2f currVelocity;
	int owner; // will be player id of who fired it
	Bullet()
		: currVelocity(0.f, 0.f), owner(-1)
	{
		this->shape.setRadius(3.f);
		this->shape.setFillColor(Color::Red);
		this->shape.setOutlineColor(Color::White);
		this->shape.setOutlineThickness(0.5f);
	}
};

class Map_Element
{
public:
	RectangleShape wall;
	Map_Element()
	{
		this->wall.setFillColor(Color::White);
		this->wall.setOutlineColor(Color::Cyan);
		this->wall.setOutlineThickness(2.f);
	}
};

class GameMap
{
public:
	int mapWidth;
	int mapHeight;
	int wallCount;
	int wallPositionX[6];
	int wallPositionY[6]; 
	float wallWidthX[6]; 
	float wallWidthY[6]; 

	GameMap()
		: mapWidth(600),
		mapHeight(600),
		wallCount(6),
		wallPositionX{ 75, 525, 150, 150, 300, 175 },
		wallPositionY{ 150, 150, 75, 525, 175, 300 },
		wallWidthX{ 5.f, 5.f, 300.f, 300.f, 5.f, 250.f },
		wallWidthY{ 300.f, 300.f, 5.f, 5.f, 250.f, 5.f }
	{}

};

class Player
{
public:
	RectangleShape box; // contains position and size
	Texture boxTexture;
	Vector2u textureSize;
	std::string username;
	int player_id;
	int killCount;
	bool alive;
	int walk;

	Player()
		: username("Nolan"), player_id(-1), killCount(0), alive(true), walk(0)
	{
		this->box.setSize(Vector2f(35.0f, 35.0f));
		this->box.setPosition(50, 50);
		this->box.setOrigin(17.5f, 17.5f);
	}
};
