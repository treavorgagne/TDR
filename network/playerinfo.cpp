#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdio>
#include <vector>

#include "../data_structures/network.hpp"

///////////////////////////////PLAYERINFO
///////////////////////////////PLAYERINFO
///////////////////////////////PLAYERINFO

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

///////////////////////////////GAMEINFO
///////////////////////////////GAMEINFO
///////////////////////////////GAMEINFO
///////////////////////////////GAMEINFO

sf::Packet& operator <<(sf::Packet& packet, Gameinfo& info){

    packet << 2 << info.num_bullets << info.num_players;


    for(int i=0; i<info.num_players; i++){
        packet << info.players[i];
    }
    for(int i=0; i<info.num_bullets; i++){
        packet << info.bullets[i];
    }

    return packet;
}



sf::Packet& operator >>(sf::Packet& packet, Gameinfo& info){

    packet >> info.type >> info.num_bullets >> info.num_players;

    for(int i=0; i<info.num_players; i++){
        Playerupdate player;
        packet >> player;
        info.players.push_back(player);
    }

    for(int i=0; i<info.num_bullets; i++){
        Bulletupdate bullet;
        packet >> bullet;
        info.bullets.push_back(bullet);
    }

    return packet;
}

void print_gameinfo(Gameinfo info){
    printf("Number of players: %d\n", info.num_players);
    printf("Number of bullets: %d\n", info.num_bullets);
    printf("Player information: \n");
    for(int i=0; i<(int)info.players.size(); i++){
        printf("    Player: %d\n", i);
        printf("        Alive: %d\n", info.players[i].alive);
        printf("        Position: (%.2f, %.2f)\n", info.players[i].posinfo.x, info.players[i].posinfo.y);
    }
    printf("Bullet information: \n");
    for(int i=0; i<(int)info.bullets.size(); i++){
        printf("    Bullet: %d\n", i);
        printf("        Position: (%.2f, %.2f)\n", info.bullets[i].pos.x, info.bullets[i].pos.y);
        printf("        Direction: (%.2f, %.2f)\n", info.bullets[i].direction.x, info.bullets[i].direction.y);
    }
    printf("Made it past bullets size of array was: %d\n", (int)info.bullets.size());
}
///////////////////////////////PLAYERUPDATE & BULLETUPDATE
///////////////////////////////PLAYERUPDATE & BULLETUPDATE
///////////////////////////////PLAYERUPDATE & BULLETUPDATE
///////////////////////////////PLAYERUPDATE & BULLETUPDATE
sf::Packet& operator >>(sf::Packet& packet, Playerupdate& info){
    return packet >> info.type >> info.alive >> info.posinfo;
}
sf::Packet& operator <<(sf::Packet& packet, Playerupdate& info){
    return packet << 1 << info.alive << info.posinfo;
}
sf::Packet& operator <<(sf::Packet& packet, Bulletupdate& info){
    return packet << 4 << info.pos << info.direction;
}
sf::Packet& operator >>(sf::Packet& packet, Bulletupdate& info){
    return packet >> info.type >> info.pos >> info.direction;
}

///////////////////////////////SPAWNINFO
///////////////////////////////SPAWNINFO
///////////////////////////////SPAWNINFO
///////////////////////////////SPAWNINFO
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
