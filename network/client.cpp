
#include "network.hpp"
#include <SFML/Network/Packet.hpp>



int ClientCommunicator::connect(std::string ip, int port){
    if(conn.connect(ip, port) != sf::Socket::Done){
        std::cout << "Error connecting to " << ip << ":" << port << std::endl;
        return 1;
    }

    std::cout << "Connected to " << ip << ":" << port << ". From port " << conn.getLocalPort() << std::endl;
    connected = true;
    return 0;
}

int ClientCommunicator::send_playerinfo(Playerinfo info){
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

Gameinfo ClientCommunicator::receive_gameinfo(){
    Gameinfo info;

    sf::Packet packet;

    if(conn.receive(packet) != sf::Socket::Done){
        std::cout << "Error recieving game information from server" << std::endl;
        exit(1);
    }

    packet >> info;

    return info;
}

Gameinitializer ClientCommunicator::receive_spawn(){
    Gameinitializer info;

    sf::Packet packet;

    if(conn.receive(packet) != sf::Socket::Done){
        std::cout << "Error recieving game information from server" << std::endl;
        exit(1);
    }

    packet >> info;

    return info;
}
