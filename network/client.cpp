
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

#include <SFML/Network.hpp>

#include <chrono>

#include <thread>

#include <string>

#include "../data_structures/network.hpp"


class ClientCommunicator {

    sf::TcpSocket conn;
    bool connected;

    public:
        int connect(std::string ip, int port);
        int send_info(Playerinfo info);

};

int ClientCommunicator::connect(std::string ip, int port){
    if(conn.connect(ip, port) != sf::Socket::Done){
        std::cout << "Error connecting to " << ip << ":" << port << std::endl;
        return 1;
    }

    std::cout << "Connected to " << ip << ":" << port << ". From port " << conn.getLocalPort() << std::endl;
    connected = true;
    return 0;
}

int ClientCommunicator::send_info(Playerinfo info){
    if(!connected){
        std::cout << "Not connected to server" << std::endl;
        return 1;
    }

    sf::Packet packet;

    packet << info;

    if (conn.send(packet) != sf::Socket::Done){
        std::cout << "Error sending packet" << std::endl;
        return 1;
    };

    return 0;
}




int main(){ //for tests

        ClientCommunicator client;

        client.connect("127.0.0.1", 35020);

        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "Sending sample position info" << std::endl;

            Playerinfo info;
            info.position.first = 24.5;
            info.position.second = 30;
            info.bullet_fired = true;
            info.bullet_direction = sf::Vector2f(23, 21.2);
            info.moving = false;

            client.send_info(info);

        }

}
