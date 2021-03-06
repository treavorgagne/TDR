#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdio>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <iostream>

#include <chrono>


#include <string>
#include <iostream>

#include <list>

#include "../data_structures/network.hpp"

class ClientCommunicator {

    sf::TcpSocket conn;
    bool connected;

    public:
        int connect(std::string ip, int port);
        int send_playerinfo(Playerinfo info);
        Gameinfo receive_gameinfo();
        Gameinitializer receive_spawn();

};

class ServerCommunicator {

    int num_players;
    int state; //0 == accepting connections, 1== playing game

    public:
        sf::SocketSelector selector;
        sf::TcpListener listener;
        std::list<sf::TcpSocket*> clients;
        void start(int port);
        int wait_for_players(int playercount);
        int accept_inputs();
        int broadcast_game_info(Gameinfo info);
        int send_game_metadata(int clientid, float spawnx, float spawny, int num_players);

};
