#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>

#include "../data_structures/network.hpp"

sf::Packet& operator <<(sf::Packet& packet, sf::Vector2f& vec){
    return packet << vec.x << vec.y;
};
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& vec){
    return packet >> vec.x >> vec.y;
};
sf::Packet& operator <<(sf::Packet& packet, std::pair<float, float>& position){
    return packet << position.first << position.second;
};
sf::Packet& operator >>(sf::Packet& packet, std::pair<float, float>& position){
    return packet >> position.first >> position.second;
};
sf::Packet& operator <<(sf::Packet& packet, MovementDirection& direction){
    return packet << (int)direction;
};
sf::Packet& operator >>(sf::Packet& packet, MovementDirection& direction){
    return packet >> direction;
};

sf::Packet& operator <<(sf::Packet& packet, Playerinfo& info){
    return packet << info.position << info.moving << info.direction << info.bullet_fired << info.bullet_direction;
};
sf::Packet& operator >>(sf::Packet& packet, Playerinfo& info){
    return packet >> info.position >> info.moving >> info.direction >> info.bullet_fired >> info.bullet_direction;
};
