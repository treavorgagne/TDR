#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdio>

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
    int dir;
    packet >> dir;
    direction = static_cast<MovementDirection>(dir);
    return packet;
};

sf::Packet& operator <<(sf::Packet& packet, Playerinfo& info){
    return packet << info.position << info.moving << info.direction << info.bullet_fired << info.bullet_direction;
};
sf::Packet& operator >>(sf::Packet& packet, Playerinfo& info){
    return packet >> info.position >> info.moving >> info.direction >> info.bullet_fired >> info.bullet_direction;
};

void print_playerinfo(Playerinfo info){

    printf("Pos (x,y): %.2f,%.2f\n", info.position.first, info.position.second);
    printf("Moving %d, Direction: %d\n", info.moving, (int)info.direction);
    printf("Bullet fired: %d, Direction (x,y): %.2f,%.2f\n", info.bullet_fired, info.bullet_direction.x,
            info.bullet_direction.y);

}
