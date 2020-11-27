#include <SFML/Network/Packet.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "game.hpp"

#include <list>
#include <vector>

//used for sending position and gunshots from client to server
//also used by server to represent each player to client
struct Playerinfo{
    std::pair<float, float> position;
    bool moving;
    MovementDirection direction;
    bool bullet_fired;
    sf::Vector2f bullet_direction;
};

struct Playerupdate{
    int playerid;
    float health;
    Playerinfo posinfo;
};

//Distributed by the server to all players minimum 4x a second
struct Gameinfo{
    int success; //If the client failed to recieve a packet, it must set this to 1 so it is ignored
    int num_players; //used so client knows how many playerinfos to expect.
    int client_id; //used so client knows which player in the vector is itself.
    std::vector<Playerupdate> players;
};

void print_playerinfo(Playerinfo info);

//includes overloads
sf::Packet& operator <<(sf::Packet& packet, sf::Vector2f& vec);
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& vec);
sf::Packet& operator <<(sf::Packet& packet, std::pair<float, float>& position);
sf::Packet& operator >>(sf::Packet& packet, std::pair<float, float>& position);
sf::Packet& operator <<(sf::Packet& packet, MovementDirection& direction);
sf::Packet& operator >>(sf::Packet& packet, MovementDirection& direction);
sf::Packet& operator <<(sf::Packet& packet, Playerinfo& info);
sf::Packet& operator >>(sf::Packet& packet, Playerinfo& info);
