#include <SFML/Network/Packet.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "game.hpp"

#include <list>

//used for sending position and gunshots from client to server
//also used by server to represent each player to client
struct Playerinfo{
    std::pair<float, float> position;
    bool moving;
    MovementDirection direction;
    bool bullet_fired;
    sf::Vector2f bullet_direction;
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
