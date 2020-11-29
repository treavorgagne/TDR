
#include "network.hpp"

int main(){

     ServerCommunicator comm;
     comm.start(35020);

     if(comm.wait_for_players(2) == 0){
        std::cout << "All players have connected!" << std::endl;
        std::cout << "Sending spawn locations" << std::endl;
        comm.send_game_metadata(0, 30, 30);
        comm.send_game_metadata(1, 560, 560);
        comm.accept_inputs();
     }


return 0;
}
