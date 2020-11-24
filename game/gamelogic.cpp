#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace sf;

enum class MovementDirection {
	up,
	down,
	left,
	right,
	up_right,
	up_left,
	down_right,
	down_left
};

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
	int wallPositionX[6]; //= {75, 525, 150, 150, 300, 175 };
	int wallPositionY[6]; //= {150, 150, 75, 525, 175, 300};
	float wallWidthX[6]; //= {5.f, 5.f, 300.f, 300.f, 5.f, 250.f};
	float wallWidthY[6]; // = {300.f, 300.f, 5.f, 5.f, 250.f, 5.f };

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

int main()
{
	// All class objects
	GameMap map;
	GameEngine eng;
	Map_Element w;
	std::vector<Map_Element> walls;
	Player p;
	std::vector<Player> player;
	Bullet b;
	std::vector<Bullet> bullets;

	srand((unsigned int) time(NULL));
	RenderWindow window(VideoMode(map.mapWidth, map.mapHeight),"TDR", (sf::Style::Titlebar | sf::Style::Close));
	window.setFramerateLimit(60);

	// adds walls to map
	for (int i = 0; i < map.wallCount; i++) {
		w.wall.setPosition((float) map.wallPositionX[i], (float) map.wallPositionY[i]);
		w.wall.setSize(Vector2f(map.wallWidthX[i], map.wallWidthY[i]));
		walls.push_back(Map_Element(w));
	}

	p.boxTexture.loadFromFile("media/3.png");
	p.boxTexture.setSmooth(true);
	p.box.setTexture(&p.boxTexture);
	p.textureSize = p.boxTexture.getSize();
	p.textureSize.x /= 4;
	p.textureSize.y /= 4;
	p.box.setTextureRect(IntRect(p.textureSize.x * 0, p.textureSize.y * 0, p.textureSize.x, p.textureSize.y));
	p.username = "T-MON3Y";
	p.player_id = 0;
	player.push_back(Player(p));
	int firerate = 0;

	//enemy 
	RectangleShape enemy;
	enemy.setSize(Vector2f(25.f, 25.f));
	enemy.setFillColor(Color::Yellow);
	std::vector<RectangleShape> enemies;
	int spawnCounter = 20;
	int spawnrate = 0;

	//vectors for aiming
	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	//cursor
	Cursor cursor;
	if (cursor.loadFromSystem(Cursor::Cross)) window.setMouseCursor(cursor);

	while (window.isOpen()) // opens game window
	{
		// needed for game 
		Event e; 
		while (window.pollEvent(e)) 
		{
			switch (e.type)
			{
			case Event::Closed:
				window.close();
				break;
			}
		}

		// firing vectorizing
		playerCenter = Vector2f(p.box.getPosition().x, p.box.getPosition().y);
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		float d = sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
		aimDirNorm = aimDir / d;

		// Client I/O manager
		Vector2f currPosition = p.box.getPosition();
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

		//Enemies
		if (spawnrate > 0) spawnrate--;
		if (spawnCounter < 20) spawnCounter++;
		if (spawnCounter >= 20 && enemies.size() < 30 && spawnrate == 0) 
		{
			bool spw = true;
			Vector2f pos = Vector2f((float) (rand() % (map.mapWidth-25)), (float) (rand() % (map.mapHeight-25)));
			
			enemy.setPosition(pos);
			for (size_t j = 0; j < walls.size(); j++) {
				if (enemy.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) spw = false;
			}
			if (spw == true) {
				enemies.push_back(RectangleShape(enemy));
				spawnCounter = 0;
				spawnrate = 125;
			}
		}

		// shooting
		if(firerate > 0) firerate--;
		if (Mouse::isButtonPressed(Mouse::Left) && (firerate == 0)) 
		{
			b.shape.setPosition(playerCenter);
			b.currVelocity = aimDirNorm * eng.BulletSpeed;
			b.owner = p.player_id;
			bullets.push_back(Bullet(b));
			firerate = eng.fireCap;
		}

		// server side collision checker
		for (size_t i = 0; i < bullets.size(); i++) {
			bullets[i].shape.move(bullets[i].currVelocity);

			// bullet wall collision
			for (size_t j = 0; j < walls.size(); j++) {
				if (bullets[i].shape.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) {
					bullets.erase(bullets.begin() + i);
				}
			}

			// bullet out of bounds
			if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > 600
				|| bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > 600)
			{
				bullets.erase(bullets.begin() + i);
			}
			else
			{
				//enemy collisions
				for (size_t k = 0; k < enemies.size(); k++) 
				{
					if (bullets[i].shape.getGlobalBounds().intersects(enemies[k].getGlobalBounds())) 
					{
						player[bullets[i].owner].killCount++;
						printf("%s killed again\nTotal Kills: %d\n", player[bullets[i].owner].username.c_str(), player[bullets[i].owner].killCount);
						bullets.erase(bullets.begin() + i);
						enemies.erase(enemies.begin() + k);
					}
				}
			}
		}

		//draw enemies, player, bullets
		window.clear();
		for (size_t i = 0; i < enemies.size(); i++) window.draw(enemies[i]);
		window.draw(p.box);
		//window.draw(player);
		for (size_t i = 0; i < bullets.size(); i++) window.draw(bullets[i].shape);
		for (size_t i = 0; i < walls.size(); i++) window.draw(walls[i].wall);
		window.display();
	}

	return 0;
}