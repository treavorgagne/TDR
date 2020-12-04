
#include "network.hpp"
#include <SFML/Network/Packet.hpp>

int main(){

    ServerCommunicator comm;
    comm.start(35020);

    printf("Testing a gameinfo pack/unpack Locally\n");

    sf::Packet packet;

    Gameinfo info;

    info.num_bullets = 2;
    info.num_players = 2;

    for(int i=1; i<3; i++){
        Playerupdate p;
        p.alive = true;
        p.posinfo = sf::Vector2f(i, i);

        info.players.push_back(p);

        Bulletupdate b;
        b.direction = sf::Vector2f(i+1, i+1);
        b.pos = sf::Vector2f(i, i);

        info.bullets.push_back(b);
    }
    printf("Packing this:\n");
    print_gameinfo(info);

    packet << info;



    Gameinfo info2;

    packet >> info2;

    printf("Unpacked this:\n");
    print_gameinfo(info2);




    if(comm.wait_for_players(2) == 0){
        std::cout << "All players have connected!" << std::endl;
        std::cout << "Sending spawn locations" << std::endl;
        comm.send_game_metadata(0, 30, 30);
        comm.send_game_metadata(1, 560, 560);
        comm.accept_inputs();
     }


return 0;
}
