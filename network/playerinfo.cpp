#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
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
    return packet << 0 << info.position << info.moving << info.direction << info.bullet_fired << info.bullet_direction;
};
sf::Packet& operator >>(sf::Packet& packet, Playerinfo& info){
    return packet >> info.type >> info.position >> info.moving >> info.direction >> info.bullet_fired >> info.bullet_direction;
};

void print_playerinfo(Playerinfo info){

    printf("Pos (x,y): %.2f,%.2f\n", info.position.first, info.position.second);
    printf("Moving %d, Direction: %d\n", info.moving, (int)info.direction);
    printf("Bullet fired: %d, Direction (x,y): %.2f,%.2f\n", info.bullet_fired, info.bullet_direction.x,
            info.bullet_direction.y);

}

sf::Packet& operator >>(sf::Packet& packet, Gameinitializer& metadata){
    return packet >> metadata.type >> metadata.client_id >> metadata.spawn_location;
};
sf::Packet& operator <<(sf::Packet& packet, Gameinitializer& metadata){
    return packet << 1 << metadata.client_id << metadata.spawn_location;
};

void print_metadata(Gameinitializer metadata){
    printf("Client id: %d\n", metadata.client_id);
    printf("Spawn location (x,y): %.2f, %.2f\n", metadata.spawn_location.first, metadata.spawn_location.second);
}

// returns the packet type so server/client know what to use to recieve it
int packet_type(sf::Packet packet){
    Playerinfo info;
    packet >> info;
    return info.type;
}
