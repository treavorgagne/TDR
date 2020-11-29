
#include "network.hpp"

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
