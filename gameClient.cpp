#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "./game/gamelogic.cpp"
#include "./network/client.cpp"
#include <math.h>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace sf;

int main()
{
	
    std::cout << "Welcome to TDR" << std::endl;

    // /* THIS CODE HERE IS THE SET UP FOR THE GAME */
    std::string ip;
    int port;

    std::cout << "Enter IP Address: ";
    std::cin >> ip;
    std::cout << "Enter Port Number: ";
    std::cin >> port;

    std::cout << "Connecting to IP address: " << ip << "" << port << std::endl;
    //connect();
    
    /* THIS CODE HERE IS FOR ONCE THE GAME START */
    
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

        p = playerMovement(p, eng);  // calls playerMovement() in gamelogic.cpp and moves player 
        p = localCollision(p, map, walls, currPosition); // calls localCollision() in gamelogic.cpp and checks for wall collisions 

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
		window.draw(p.box); // this will be changed to update all players
		for (size_t i = 0; i < bullets.size(); i++) window.draw(bullets[i].shape);
		for (size_t i = 0; i < walls.size(); i++) window.draw(walls[i].wall);
		window.display();
	}

	return 0;
}