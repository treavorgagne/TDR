#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "./game/gamelogic.hpp"
#include "network/network.hpp"
#include <math.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "audio/SoundPlayer.hpp"

using namespace sf;

int main()
{
	// /* THIS CODE HERE IS THE SET UP FOR THE GAME */
	
	// All Objects needed for game
	int player_id = 0;
	int firerate = 0;
	GameMap map;
	GameEngine eng;
	Map_Element w;
	std::vector<Map_Element> walls;
	Player p;
	std::vector<Player> player;
	Bullet b;
	std::vector<Bullet> bullets;
	ClientCommunicator client;
	SoundPlayer playSound;
	
	std::string name;
	std::string ip;
    int port;

	std::cout << "Welcome to TDR" << std::endl;
	std::cout << "Enter Username (no spaces) : ";
	std::cin >> name;
    std::cout << "Enter IP Address: ";
    std::cin >> ip;
    std::cout << "Enter Port Number: ";
    std::cin >> port;
    std::cout << "Connecting to IP address: " << ip << " And Port Number: " << port << std::endl;
    client.connect(ip, port);
	
	srand((unsigned int) time(NULL));

	// adds walls to map
	for (int i = 0; i < map.wallCount; i++) {
		w.wall.setPosition((float) map.wallPositionX[i], (float) map.wallPositionY[i]);
		w.wall.setSize(Vector2f(map.wallWidthX[i], map.wallWidthY[i]));
		walls.push_back(Map_Element(w));
	}
	



	//vectors for aiming
	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	Gameinitializer spawn = client.receive_spawn();
	std::cout << "Cliend_id: " <<spawn.client_id;
	std::cout << "Waiting for response with game metadata" << std::endl;
	std::cout << "Got my spawn information:" << std::endl;
	// print_metadata(spawn);
	// printf("\n");
	player_id = spawn.client_id;

	RenderWindow window(VideoMode(map.mapWidth, map.mapHeight),"TDR", (sf::Style::Titlebar | sf::Style::Close));
	window.setFramerateLimit(60);
	
	Cursor cursor;
	if (cursor.loadFromSystem(Cursor::Cross)) window.setMouseCursor(cursor);
	

	std::cout << "Going into pre-game lobby to wait for the remaining players to join:" << std::endl;
	while(player.size() < 3){

		//receive other players information and push them back;
		p.boxTexture.loadFromFile("media/25.png");
		p.boxTexture.setSmooth(true);
		p.box.setTexture(&p.boxTexture);
		p.textureSize = p.boxTexture.getSize();
		p.textureSize.x /= 4;
		p.textureSize.y /= 4;
		p.box.setTextureRect(IntRect(p.textureSize.x * 0, p.textureSize.y * 0, p.textureSize.x, p.textureSize.y));
		//code to set player info and position
		//p.player_id = spawn.client_id;
		//p.username = name;
		p.box.setPosition((float) (rand() % (map.mapWidth-25)), (float) (rand() % (map.mapHeight-25)));
		player.push_back(Player(p));


		Event e;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
  				break;
			}
		}

	}
	// code to client player texture
	std::string num = std::to_string( (int) (rand() % 24) + 1);
	std::string skin = "media/";
	std::string end = ".png";
	skin.append(num);
	skin.append(end);
	player[player_id].boxTexture.loadFromFile(skin);
	player[player_id].boxTexture.setSmooth(true);
	player[player_id].box.setTexture(&player[player_id].boxTexture);
	player[player_id].textureSize = player[player_id].boxTexture.getSize();
	player[player_id].textureSize.x /= 4;
	player[player_id].textureSize.y /= 4;
	player[player_id].box.setTextureRect(IntRect(player[player_id].textureSize.x * 0, player[player_id].textureSize.y * 0, player[player_id].textureSize.x, player[player_id].textureSize.y));
	//code to set player info and position
	player[player_id].player_id = spawn.client_id;
	player[player_id].username = name;
	player[player_id].box.setPosition(std::get<0>(spawn.spawn_location), std::get<1>(spawn.spawn_location));
	player[player_id].username = name;

	/* THIS CODE HERE IS FOR ONCE THE GAME START */
	
	printf("\n");
	std::cout << "--GAME COUNTDOWN--" << std::endl;
	std::cout << "--------5---------" << std::endl;
	usleep(1000000);
	std::cout << "--------4---------" << std::endl;
	usleep(1000000);
	std::cout << "--------3---------" << std::endl;
	usleep(1000000);
	std::cout << "--------2---------" << std::endl;
	usleep(1000000);
	std::cout << "--------1---------" << std::endl;
	usleep(1000000);
	std::cout << "------START-------" << std::endl;
	printf("\n");

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
			default:
  				break;
			}
		}
		

		if( player[player_id].alive ){

			// Client I/O manager
			Vector2f currPosition = player[player_id].box.getPosition();
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player[player_id].box.move(-eng.movementSpeed, 0.0f);
				player[player_id].box.setTextureRect(IntRect(player[player_id].textureSize.x * (player[player_id].walk++ % 4), player[player_id].textureSize.y * 1, player[player_id].textureSize.x, player[player_id].textureSize.y));
			}
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player[player_id].box.move(0.0f, -eng.movementSpeed);
				player[player_id].box.setTextureRect(IntRect(player[player_id].textureSize.x * (player[player_id].walk++ % 4), player[player_id].textureSize.y * 3, player[player_id].textureSize.x, player[player_id].textureSize.y));
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player[player_id].box.move(eng.movementSpeed, 0.0f);
				player[player_id].box.setTextureRect(IntRect(player[player_id].textureSize.x * (player[player_id].walk++ % 4), player[player_id].textureSize.y * 2, player[player_id].textureSize.x, player[player_id].textureSize.y));
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player[player_id].box.move(0.0f, eng.movementSpeed);
				player[player_id].box.setTextureRect(IntRect(player[player_id].textureSize.x * (player[player_id].walk++ % 4), player[player_id].textureSize.y * 0, player[player_id].textureSize.x, player[player_id].textureSize.y));
			}

			//Client I/O manager
			if (player[player_id].box.getPosition().x < 0) player[player_id].box.setPosition(0, player[player_id].box.getPosition().y);
			if (player[player_id].box.getPosition().x > map.mapWidth) player[player_id].box.setPosition((float) map.mapWidth, player[player_id].box.getPosition().y);
			if (player[player_id].box.getPosition().y < 0) player[player_id].box.setPosition(player[player_id].box.getPosition().x, 0);
			if (player[player_id].box.getPosition().y > map.mapHeight) player[player_id].box.setPosition(player[player_id].box.getPosition().x, (float) map.mapHeight);
			for (size_t j = 0; j < walls.size(); j++) {
				if (player[player_id].box.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) {
					player[player_id].box.setPosition(currPosition);
				}
			}

			// client firing vectorizing and shooting 
			playerCenter = Vector2f(player[player_id].box.getPosition().x, player[player_id].box.getPosition().y);
			mousePosWindow = Vector2f(Mouse::getPosition(window));
			aimDir = mousePosWindow - playerCenter;
			float d = sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
			aimDirNorm = aimDir / d;

			if(firerate > 0) firerate--;
			if (Mouse::isButtonPressed(Mouse::Left) && (firerate == 0))
			{
				b.shape.setPosition(playerCenter);
				b.currVelocity = aimDirNorm * eng.BulletSpeed;
				b.owner = player_id;
				bullets.push_back(Bullet(b));
				firerate = eng.fireCap;
				//audio pew pew
				playSound.playWalk();
						


			}

		}
		

		// server side collision checker
		for (size_t i = 0; i < bullets.size(); i++) {
			bullets[i].shape.move(bullets[i].currVelocity);

			// bullet wall collision
			for (size_t j = 0; j < walls.size(); j++) {
				if (bullets[i].shape.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) {
					bullets.erase(bullets.begin() + i);
					//audio thud
					
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
				// bullet player collision detector
				for(size_t k = 0; k < player.size(); k++)
				{
					if( ( bullets[i].shape.getGlobalBounds().intersects(player[k].box.getGlobalBounds()) ) && ( bullets[i].owner != (int) k ))
					{
						player[bullets[i].owner].killCount++;
						bullets.erase(bullets.begin() + i);
						player[k].alive = false;
						player[k].lives -= 1;
					}

				}
			}
		}

		//respawn player 
		for (size_t i = 0; i < player.size(); i++){ 

			 if(!player[i].alive && player[i].lives > 0){
				// respawn; 
				bool spw = true;
				Vector2f pos = Vector2f((float) (rand() % (map.mapWidth-25)), (float) (rand() % (map.mapHeight-25)));
				player[i].box.setPosition(pos);

				for (size_t j = 0; j < walls.size(); j++) {
					if (player[i].box.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) spw = false;
				}
				if (spw == true) {
					player[i].alive = true;
				}

			 } 
		}

		// update game
		window.clear();
		for (size_t i = 0; i < player.size(); i++){
			 if(player[i].alive) window.draw(player[i].box);
		}
		for (size_t i = 0; i < bullets.size(); i++) window.draw(bullets[i].shape);
		for (size_t i = 0; i < walls.size(); i++) window.draw(walls[i].wall);
		window.display();
	}

	return 0;
}
