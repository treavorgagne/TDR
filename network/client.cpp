
#include <iostream>

#include <SFML/Network.hpp>


#include <chrono>

#include<thread>

int main(){


	sf::TcpSocket sock;

        sock.connect("127.0.0.1", 35020);

        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Sending a keydown command" << std::endl;

            const char keydown[] = "keydown";

            sock.send(keydown, sizeof(keydown));

            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Sending a key-up command" << std::endl;

            const char keyup[] = "keyup";

            sock.send(keyup, sizeof(keyup));

        }




}
