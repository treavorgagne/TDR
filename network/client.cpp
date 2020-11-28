
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
        info.success = 1;
    }

    //packet >> info;

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

int main(){ //for tests

         ClientCommunicator client;

         if(client.connect("127.0.0.1", 35020) != 0){
             return 1;
         }
         std::cout << "Waiting for response with game metadata" << std::endl;

         Gameinitializer spawn = client.receive_spawn();

         std::cout << "Got the game information:" << std::endl;

         print_metadata(spawn);

         printf("\n");


         while(1){
             std::this_thread::sleep_for(std::chrono::seconds(2));


             std::cout << "Sending sample position info:" << std::endl;

             Playerinfo info;
             info.position.first = 24.5;
             info.position.second = 30;
             info.bullet_fired = true;
             info.bullet_direction = sf::Vector2f(23, 21.2);
             info.moving = false;
             info.direction = MovementDirection::left;
             print_playerinfo(info);

             client.send_playerinfo(info);



             printf("\n");

         }

 }
