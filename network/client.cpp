
#include <iostream>

#include <SFML/Network.hpp>


#include <chrono>

#include <thread>

int main(){


	sf::TcpSocket sock;

        sock.connect("127.0.0.1", 35020);

        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Sending data" << std::endl;

            const char keydown[] = "keydown";

            sf::Packet packet;

            packet << keydown;

            if(sock.send(packet) != sf::Socket::Done){
                std::cout << "Error sending data" << std::endl;
            }


        }




}
