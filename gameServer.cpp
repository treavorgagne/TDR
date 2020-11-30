#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "./game/gamelogic.hpp"
#include "network/network.hpp"
#include <math.h>
#include <iostream>
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

    public:
        int setup_game(int port);
        int start_game();
        void render_game();//Used to observe the game from server(start in seperate thread if using)
        int check_collisions();
        int fire_bullet(int player_id, Vector2f direction); //call if player send in bullet fired flag
        int respawn_dead_players();
};



void Server::render_game(){
    RenderWindow window(VideoMode(map.mapWidth, map.mapHeight),"TDR", (sf::Style::Titlebar | sf::Style::Close));
    window.setFramerateLimit(60);
    // update game
    while(1){
        window.clear();
        for (size_t i = 0; i < players.size(); i++){
                 if(players[i].alive) window.draw(players[i].box);
        }
        for (size_t i = 0; i < bullets.size(); i++) window.draw(bullets[i].shape);
        for (size_t i = 0; i < walls.size(); i++) window.draw(walls[i].wall);
        window.display();
    }
}
//setup and wait for players
int Server::setup_game(int port){

    // adds walls to map
    for (int i = 0; i < map.wallCount; i++) {
            w.wall.setPosition((float) map.wallPositionX[i], (float) map.wallPositionY[i]);
            w.wall.setSize(Vector2f(map.wallWidthX[i], map.wallWidthY[i]));
            walls.push_back(Map_Element(w));
    }

    //wait for players to connect;
    comm.start(port);
    comm.wait_for_players(2);
    //set player spawns
    set_spawns();


    return 0;
}


//decide where players will spawn and send spawn info
int Server::set_spawns(){


    for(std::vector<Player>::iterator it = players.begin(); it<players.end(); it++){
        int player_id = it - players.begin();

        players[player_id].player_id = player_id;

        //TODO: SET SPAWN LOCATIONS BASED ON PLAYER ID(Verify this works)
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
        if(!players[i].alive && players[i].lives > 0){
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


int Server::fire_bullet(int player_id, Vector2f direction){

    Player cur = players[player_id];
    Vector2f pos = cur.box.getPosition();
    Bullet b;

    b.shape.setPosition(pos);
    b.currVelocity = direction * eng.BulletSpeed;
    b.owner = player_id;
    bullets.push_back(Bullet(b));

    return 0;
};

int main(){
    Server server;
    server.setup_game(35020);
//    std::thread render_loop(&Server::render_game, server);


//    render_loop.join();
    return 0;
}
