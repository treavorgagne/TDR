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
#include <thread>
#include <unistd.h>
#include "audio/SoundPlayer.hpp"

using namespace sf;

class Client{

    int player_id = 0;
	int firerate = 0;

	GameMap map;
	GameEngine eng;
	Map_Element w;
	Bullet b;
	Player p;
	std::vector<Map_Element> walls;
	std::vector<Player> player;
	std::vector<Bullet> bullets;
	ClientCommunicator client;
	SoundPlayer playSound;

    //for networking
    Vector2f last_bullet_velocity;
	bool bullet_fired;
    Gameinfo latest_gameinfo;
    bool new_gameinfo;
    void apply_gameinfo(Gameinfo info);
    void recieve_loop();

    //internal functions
    void check_movement();
    void check_bullet_fired(Vector2f mousepos);
    void set_texture(int player_id, int texture);

    public:
        void pre_game();
		void game_loop();
        void initialize();
};

void Client::initialize(){
    new_gameinfo = false;
    bullet_fired = false;
    last_bullet_velocity = Vector2f(0, 0);

    //Init RNG
	srand((unsigned int) time(NULL));

	// adds walls to map
	for (int i = 0; i < map.wallCount; i++) {
		w.wall.setPosition((float) map.wallPositionX[i], (float) map.wallPositionY[i]);
		w.wall.setSize(Vector2f(map.wallWidthX[i], map.wallWidthY[i]));
		walls.push_back(Map_Element(w));
	}
}

void Client::set_texture(int id, int texture){
	std::string skin = "media/";
	std::string end = ".png";
	skin.append(std::to_string(texture));
	skin.append(end);
	player[id].boxTexture.loadFromFile(skin);
	player[id].boxTexture.setSmooth(true);
	player[id].box.setTexture(&player[id].boxTexture);
	player[id].textureSize = player[id].boxTexture.getSize();
	player[id].textureSize.x /= 4;
	player[id].textureSize.y /= 4;
	player[id].box.setTextureRect(IntRect(player[id].textureSize.x * 0,
                player[id].textureSize.y * 0, player[id].textureSize.x, player[id].textureSize.y));
}


void Client::pre_game(){

	std::string ip;
    int port;
	
	std::cout << "Welcome to TDR" << std::endl;
    std::cout << "Enter IP Address: ";
    std::cin >> ip;
    std::cout << "Enter Port Number: ";
    std::cin >> port;
    std::cout << "Connecting to IP address: " << ip << " And Port Number: " << port << std::endl;
    client.connect(ip, port);

	Gameinitializer spawn = client.receive_spawn();
	std::cout << "My player id: " << spawn.client_id << std::endl;
	std::cout << "Waiting for response with game metadata" << std::endl;
	std::cout << "Got my spawn information" << std::endl;
	// print_metadata(spawn);
	// printf("\n");
	player_id = spawn.client_id;

	std::cout << "Going into pre-game lobby to wait for the remaining players to join:" << std::endl;

    //make all the players and give them random spawns for now
    //They will get updated locations later
	while(player.size() < 3){
		p.box.setPosition((float) (rand() % (map.mapWidth-25)), (float) (rand() % (map.mapHeight-25)));
		player.push_back(Player(p));
	}

    //make everyone a zombie
    for(size_t i=0; i<player.size(); i++){
        set_texture(i, 25);
    }
    //re-set local user texture randomly
	int num = (int) (rand() % 24) + 1;
    set_texture(player_id, num);

	//code to set player info and position
	player[player_id].player_id = spawn.client_id;
	player[player_id].box.setPosition(std::get<0>(spawn.spawn_location), std::get<1>(spawn.spawn_location));

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
}

void Client::check_movement(){
			// Client I/O manager
    Vector2f currPosition = player[player_id].box.getPosition();

    unsigned int next_texture = player[player_id].textureSize.y;
    bool change_text = false;


    if (Keyboard::isKeyPressed(Keyboard::A)){
        player[player_id].box.move(-eng.movementSpeed, 0.0f);
        next_texture = 1;
        change_text = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::W)){
        player[player_id].box.move(0.0f, -eng.movementSpeed);
        next_texture = 3;
        change_text = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::D)){
        player[player_id].box.move(eng.movementSpeed, 0.0f);
        next_texture = 2;
        change_text = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::S)){
        player[player_id].box.move(0.0f, eng.movementSpeed);
        next_texture = 0;
        change_text = true;
    }

    if(change_text){
        player[player_id].box.setTextureRect(
                IntRect(player[player_id].textureSize.x * (player[player_id].walk++ % 4),
                player[player_id].textureSize.y * next_texture, player[player_id].textureSize.x,
                player[player_id].textureSize.y)
                );

    }

    if (player[player_id].box.getPosition().x < 0)
        player[player_id].box.setPosition(0, player[player_id].box.getPosition().y);

    if (player[player_id].box.getPosition().x > map.mapWidth)
        player[player_id].box.setPosition((float) map.mapWidth, player[player_id].box.getPosition().y);

    if (player[player_id].box.getPosition().y < 0)
        player[player_id].box.setPosition(player[player_id].box.getPosition().x, 0);

    if (player[player_id].box.getPosition().y > map.mapHeight)
        player[player_id].box.setPosition(player[player_id].box.getPosition().x, (float) map.mapHeight);


    //check for walls before moving
    for (size_t j = 0; j < walls.size(); j++) {
        if (player[player_id].box.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) {
            player[player_id].box.setPosition(currPosition);
        }
    }
}

void Client::check_bullet_fired(Vector2f mousePosWindow){

	Vector2f playerCenter;
	Vector2f aimDir;
	Vector2f aimDirNorm;
    // client firing vectorizing and shooting
    playerCenter = Vector2f(player[player_id].box.getPosition().x, player[player_id].box.getPosition().y);
    aimDir = mousePosWindow - playerCenter;
    float d = sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
    aimDirNorm = aimDir / d;

    if(firerate > 0) firerate--;

    if (Mouse::isButtonPressed(Mouse::Left) && (firerate == 0))
    {
        b.shape.setPosition(playerCenter);
        b.currVelocity = aimDirNorm * eng.BulletSpeed;
        last_bullet_velocity = b.currVelocity;
        b.owner = player_id;
        bullets.push_back(Bullet(b));
        firerate = eng.fireCap;
        bullet_fired=true;
        //audio pew pew
		playSound.playShoot();
    }


}

void Client::game_loop(){
	//vectors for aiming


	RenderWindow window(VideoMode(map.mapWidth, map.mapHeight),"TDR", (sf::Style::Titlebar | sf::Style::Close));
	window.setFramerateLimit(60);

    std::thread inputs(&Client::recieve_loop, this);

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
			default:
  				break;
			}
		}

		if( player[player_id].alive ){
            check_movement();
            check_bullet_fired(Vector2f(Mouse::getPosition(window)));
		}

		// update game
        Playerinfo info;

        if (bullet_fired){
            info.bullet_fired = true;
            info.bullet_direction = last_bullet_velocity;
            bullet_fired = false;
        }else{
            info.bullet_fired = false;
        }


        Vector2f pos = player[player_id].box.getPosition();
        info.position.first = pos.x;
        info.position.second = pos.y;

        if(client.send_playerinfo(info) != 0){
            exit(1);
        }

        if(new_gameinfo){
            new_gameinfo = false;
            apply_gameinfo(latest_gameinfo);
        }

		window.clear();
		for (size_t i = 0; i < player.size(); i++){
			 if(player[i].alive) window.draw(player[i].box);
		}
		for (size_t i = 0; i < bullets.size(); i++) window.draw(bullets[i].shape);
		for (size_t i = 0; i < walls.size(); i++) window.draw(walls[i].wall);
		window.display();
	}
}

void Client::apply_gameinfo(Gameinfo info){


    //create the bullets
    std::vector<Bullet> newbulletvec;
    for(size_t i=0; i<info.bullets.size(); i++){
        Bullet b;
        b.owner = player_id;
        b.currVelocity = info.bullets[i].direction;
        b.shape.setPosition(info.bullets[i].pos);

        newbulletvec.push_back(b);
    }

    //Update the players

    for(size_t i=0; i<info.players.size(); i++){
        if((int)i != player_id){ //don't overwrite my own position with old info
            player[i].box.setPosition(info.players[i].posinfo);
        }
            player[i].alive = info.players[i].alive;
    }

    //
    // Assign the bullets
    bullets = newbulletvec;

}


void Client::recieve_loop(){

    while(1){
        Gameinfo game = client.receive_gameinfo();

        printf("Recieved this game information\n");

        print_gameinfo(game);

        //I don't want to use a mutex so I'm doing this
        //The client will fetch it each fram an update arrives
        latest_gameinfo = game;
        new_gameinfo = true;


    }


}


int main()
{

	Client c;
    c.initialize();
	c.pre_game();
	c.game_loop();

	return 0;
}
