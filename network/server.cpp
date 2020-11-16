
#include <iostream>
#include <SFML/Network.hpp>

#include <thread>

#include <list>


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

};


void ServerCommunicator::start (int port){
    listener.listen(port);
    selector.add(listener);
    num_players = 0;
}

int ServerCommunicator::wait_for_players(int playercount){
    state = 0;
    while(num_players < playercount){
        if(selector.wait()){ //block until someone requests a conneciton
            if (selector.isReady(listener)){ //incoming connection
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done){
                        clients.push_back(client); //add client to connections
                        std::cout << "New connection received from " << client->getRemoteAddress() << std::endl;
                        selector.add(*client); //add client to selector
                        num_players++;

                }
                else{
                    std::cout << "Error connecting to a client" << std::endl;
                    delete client;
                }
            }
        }
        else{
            if(state!=0){
                return 1;
            }

        }
    }

    return 0;
}
int ServerCommunicator::accept_inputs(){
    state = 1;
    while(state == 1){
        //iterate through the connections and check for data
        selector.wait();
        for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it){
            sf::TcpSocket& client = **it;
            if (selector.isReady(client)){
                // The client has sent some data, we can receive it
                sf::Packet packet;
                if (client.receive(packet) == sf::Socket::Done){
                    std::cout << "Recieved some data from client" << std::endl;
                }
            }
        }


    }

    return 0;
}

int main(){

     ServerCommunicator comm;
     comm.start(35020);

     if(comm.wait_for_players(2) == 0){
        std::cout << "all players have connected!" << std::endl;
        comm.accept_inputs();
     }


return 0;
}