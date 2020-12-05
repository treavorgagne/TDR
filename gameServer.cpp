#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "./game/gamelogic.hpp"
#include "network/network.hpp"
#include <math.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <string>
#include <unistd.h>

using namespace sf;
class Server{
    GameMap map;
    GameEngine eng;
    Map_Element w;
    std::vector<Map_Element> walls;
    std::vector<Player> players;
    std::vector<Bullet> bullets;
    ServerCommunicator comm;

    int set_spawns();
    int update_player(Playerinfo info, int client);

    public:
        int setup_game(int port, int player_count);
        int start_game();
        void main_loop();
        int check_collisions();
        void input_loop();
        int respawn_dead_players();
        Gameinfo make_gameinfo();
};

//package up the game information to send to everyone
Gameinfo Server::make_gameinfo(){

    Gameinfo info;

    info.type = 2;
    info.num_players = (int)players.size();
    info.num_bullets = (int)bullets.size();

    //get player positions
    for(int i=0; i<(int)players.size(); i++){
        Playerupdate p;
        p.type = 1;
        p.alive = players[i].alive;
        p.posinfo = players[i].box.getPosition();

        info.players.push_back(p);
    }

    //get bullet positions and velocities
    for(int i=0; i<(int)bullets.size(); i++){
        printf("Adding a bullet to gameinfo\n");
        Bulletupdate b;
        b.type = 4;
        b.pos = bullets[i].shape.getPosition();
        b.direction = bullets[i].currVelocity;

        info.bullets.push_back(b);
    }

    print_gameinfo(info);

    return info;
}

int Server::update_player(Playerinfo info, int client){
   // printf("Client %d\n", client);
   // print_playerinfo(info);

    Bullet b;

    if(!players[client].alive){
        return 0;
    }

    if(info.bullet_fired == 1){
        Vector2f PlayerCenter = Vector2f(players[client].box.getPosition().x, players[client].box.getPosition().y);
        b.shape.setPosition(PlayerCenter);
        b.currVelocity = info.bullet_direction;
        b.owner = client;
        bullets.push_back(Bullet(b));
    }
    players[client].box.setPosition(info.position.first, info.position.second);


    return 0;
}

void Server::input_loop(){
    std::cout << "Now accepting inputs" << std::endl;
    printf("\n");
    comm.selector.remove(comm.listener); //remove listener to reject new connections
    comm.listener.close();

    while(true){
        //iterate through the connections and check for data
        if (comm.clients.size() == 0){
            std::cout << "All clients disconnected" << std::endl;
            exit(1);
        }


        comm.selector.wait(); //wait for a socket to be ready
        int client_no = 0;
        for (std::list<sf::TcpSocket*>::iterator it = comm.clients.begin(); it != comm.clients.end(); ++it){

            sf::TcpSocket& client = **it;

            if (comm.selector.isReady(client)){
                // This client has sent some data, we can receive it
                sf::Packet packet;
                sf::Socket::Status status = client.receive(packet);

                if (status == sf::Socket::Done){
                    //extract packet
                    //std::cout << "Packet recieved from client on port: " << client.getRemotePort() << std::endl;

                    int type = packet_type(packet);
                    //std::cout << "packet type is: " << type << std::endl;

                    if(type == 0){
                        Playerinfo info;
                        packet >> info;
                        update_player(info, client_no);
                    }
                    else {
                        std::cout << "Unkown packet type" << std::endl;
                    }


                }
                else if(status == sf::Socket::Disconnected){
                    std::cout << "Client has been disconnected" << std::endl;
                    //code to remove client here
                    comm.selector.remove(client);
                    client.disconnect();
                    it = comm.clients.erase(it);

                    std::cout << "Clients remaining: " << comm.clients.size() << std::endl;

                }
                else{
                    std::cout << "Error recieving data from client: " << status << std::endl;
                }

            }

          client_no++;
        }


    }

}


void Server::main_loop(){
    RenderWindow window(VideoMode(map.mapWidth, map.mapHeight),
                "TDR", (sf::Style::Titlebar | sf::Style::Close));
    window.setFramerateLimit(60);

    //start thread to accept new inputs
    //
    std::thread inputs(&Server::input_loop, this);
    //
    //
    int frame = 0;

    // update game
    while(1){
        //Collisions
        check_collisions();

        //Send game updates
        Gameinfo info = make_gameinfo();

        frame++;
        if(frame % 1 == 0){
            comm.broadcast_game_info(info);
        }

        if(frame > 1000) frame=0;



        //render it
        window.clear();
        for (size_t i = 0; i < players.size(); i++){
                 if(players[i].alive) window.draw(players[i].box);
        }
        for (size_t i = 0; i < bullets.size(); i++) window.draw(bullets[i].shape);
        for (size_t i = 0; i < walls.size(); i++) window.draw(walls[i].wall);
        window.display();
    }

    inputs.join();
}

//setup and wait for players
int Server::setup_game(int port, int player_count){

    // adds walls to map
    for (int i = 0; i < map.wallCount; i++) {
            w.wall.setPosition((float) map.wallPositionX[i], (float) map.wallPositionY[i]);
            w.wall.setSize(Vector2f(map.wallWidthX[i], map.wallWidthY[i]));
            walls.push_back(Map_Element(w));
    }

    //wait for players to connect;
    comm.start(port);
    comm.wait_for_players(player_count);

    //add players to game

    for(int i=0; i<3; i++){
        Player p;
        p.alive = true;
        p.lives = 2;
        p.player_id = i;
        p.username = i;
        players.push_back(p);
    }

    //set textures
    for(size_t i=0; i<players.size(); i++){
        std::string num = std::to_string((int) (rand() % 24) + 1);
        std::string skin = "media/";
        std::string end = ".png";
        skin.append(num);
        skin.append(end);
        std::cout << "Loading texture from file" << skin << std::endl;
        players[i].boxTexture.loadFromFile(skin);
        players[i].boxTexture.setSmooth(true);
        players[i].box.setTexture(&players[i].boxTexture);
        players[i].textureSize = players[i].boxTexture.getSize();
        players[i].textureSize.x /= 4;
        players[i].textureSize.y /= 4;
        players[i].box.setTextureRect(IntRect(players[i].textureSize.x * 1, players[i].textureSize.y * 1,
                    players[i].textureSize.x, players[i].textureSize.y));
    }
    //set player spawns
    set_spawns();

    return 0;
}


//decide where players will spawn and send spawn info
int Server::set_spawns(){


    for(std::vector<Player>::iterator it = players.begin(); it<players.end(); it++){
        int player_id = it - players.begin();

        players[player_id].player_id = player_id;

        Vector2f pos = Vector2f((float) (rand() % (map.mapWidth-25)),
                (float) (rand() % (map.mapHeight-25)));

        players[player_id].box.setPosition(pos.x, pos.y);

        comm.send_game_metadata(player_id, pos.x, pos.y);

    }

    return 0;
};

int Server::respawn_dead_players(){

    //respawn all players that are dead and have lives left
    for (size_t i = 0; i < players.size(); i++){
        while(!players[i].alive && players[i].lives > 0){
            bool spw = true;
            Vector2f pos = Vector2f((float) (rand() % (map.mapWidth-25)), (float) (rand() % (map.mapHeight-25)));
            players[i].box.setPosition(pos);
            for (size_t j = 0; j < walls.size(); j++) {
                if (players[i].box.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) spw = false;
            }
            if (spw == true) {
                players[i].alive = true;
            }
         }
    }

    return 0;
};

// server side collision checker
int Server::check_collisions(){

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
                || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > 600){
                bullets.erase(bullets.begin() + i);
        }
        else{ //check if it hit a player

            for(size_t k = 0; k < players.size(); k++){
                if( (bullets[i].shape.getGlobalBounds().intersects(players[k].box.getGlobalBounds()) )
                        && ( bullets[i].owner != (int) k )){
                        //bullets can't hit the person who fired them?
                        players[bullets[i].owner].killCount++;
                        bullets.erase(bullets.begin() + i);
                        players[k].alive = false;
                        players[k].lives -= 1;
                }
            }
        }
    }

    return 0;
};




int main(){
    Server server;
    server.setup_game(35020, 3);

    server.main_loop();

    return 0;
}
