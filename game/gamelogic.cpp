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
		: killCount(0), alive(true), walk(0), player_id(-1), username("Nolan")
	{
		this->box.setSize(Vector2f(35.0f, 35.0f));
		this->box.setPosition(50, 50);
		this->box.setOrigin(17.5f, 17.5f);
	}
};


Player localCollision(Player p, GameMap map, std::vector<Map_Element> walls, Vector2f currPosition){

		// local collision detection between player, boundary, and walls
		if (p.box.getPosition().x < 0) p.box.setPosition(0, p.box.getPosition().y);
		if (p.box.getPosition().x > map.mapWidth) p.box.setPosition((float) map.mapWidth, p.box.getPosition().y);
		if (p.box.getPosition().y < 0) p.box.setPosition(p.box.getPosition().x, 0);
		if (p.box.getPosition().y > map.mapHeight) p.box.setPosition(p.box.getPosition().x, (float) map.mapHeight);
		for (size_t j = 0; j < walls.size(); j++) {
			if (p.box.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) {
				p.box.setPosition(currPosition);
			}
		}

		return p;

}

Player playerMovement(Player p, GameEngine eng){

		// Client I/O manager
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			p.box.move(-eng.movementSpeed, 0.0f);
			p.box.setTextureRect(IntRect(p.textureSize.x * (p.walk++ % 4), p.textureSize.y * 1, p.textureSize.x, p.textureSize.y));
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			p.box.move(0.0f, -eng.movementSpeed);
			p.box.setTextureRect(IntRect(p.textureSize.x * (p.walk++ % 4), p.textureSize.y * 3, p.textureSize.x, p.textureSize.y));
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			p.box.move(eng.movementSpeed, 0.0f);
			p.box.setTextureRect(IntRect(p.textureSize.x * (p.walk++ % 4), p.textureSize.y * 2, p.textureSize.x, p.textureSize.y));
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			p.box.move(0.0f, eng.movementSpeed);
			p.box.setTextureRect(IntRect(p.textureSize.x * (p.walk++ % 4), p.textureSize.y * 0, p.textureSize.x, p.textureSize.y));
		}

		return p;

}
