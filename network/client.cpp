
#include "network.hpp"



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

Gameinfo ClientCommunicator::receive_gameinfo(){
    Gameinfo info;

    sf::Packet packet;

    if(conn.receive(packet) != sf::Socket::Done){
        std::cout << "Error recieving game information from server" << std::endl;
        info.success = 1;
    }

    return info;
}


// int main(){ //for tests

//         ClientCommunicator client;

//         if(client.connect("127.0.0.1", 35020) != 0){
//             return 1;
//         }

//         while(1){
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             std::cout << "Sending sample position info:" << std::endl;

//             Playerinfo info;
//             info.position.first = 24.5;
//             info.position.second = 30;
//             info.bullet_fired = true;
//             info.bullet_direction = sf::Vector2f(23, 21.2);
//             info.moving = false;
//             info.direction = MovementDirection::left;

//             print_playerinfo(info);

//             client.send_info(info);

//             printf("\n");

//         }

// }
