
#include "network.hpp"

#include <chrono>
#include <thread>

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

         Playerinfo info;
         info.position.first = spawn.spawn_location.first;
         info.position.second = spawn.spawn_location.second;
         info.bullet_fired = true;
         info.bullet_direction = sf::Vector2f(23, 21.2);
         info.moving = false;
         info.direction = MovementDirection::left;

         int counter = 0;

        //walk left and right quickly(sync to 60 sends/s)
         while(1){
             std::this_thread::sleep_for(std::chrono::milliseconds(17));

             std::cout << "Sending sample position info:" << std::endl;

             print_playerinfo(info);

             if(client.send_playerinfo(info) != 0){
                 exit(1);
             };

             counter++;

             if(counter < 60){
                info.position.first += 1;
             }else if(counter > 60){
                counter = 0;
                info.position.first -= 60;
             }

         }

 }
