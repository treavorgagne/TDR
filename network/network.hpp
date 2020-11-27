#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdio>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <iostream>

#include <chrono>

#include <thread>

#include <string>
#include <iostream>

#include <list>

#include "../data_structures/network.hpp"

class ClientCommunicator {

    sf::TcpSocket conn;
    bool connected;

    public:
        int connect(std::string ip, int port);
        int send_info(Playerinfo info);
        Gameinfo receive_gameinfo();

};

class ServerCommunicator {

    sf::TcpListener listener;
    std::list<sf::TcpSocket*> clients;
    sf::SocketSelector selector;
    int num_players;
    int state; //0 == accepting connections, 1== playing game

    public:
        void start(int port);
        int wait_for_players(int playercount);
        int accept_inputs();
        int broadcast_game_info(Gameinfo info);

};
