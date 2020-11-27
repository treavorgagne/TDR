
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <iostream>
#include <SFML/Network.hpp>

#include <thread>

#include <list>

#include "../data_structures/network.hpp"


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
        int broadcast_game_info(Gameinfo info);

};


void ServerCommunicator::start (int port){
    listener.listen(port);
    selector.add(listener);
    num_players = 0;
    std::cout << "Started server on port " << listener.getLocalPort() << std::endl;
}

int ServerCommunicator::wait_for_players(int playercount){
    state = 0;
    while(num_players < playercount){
        if(selector.wait()){ //block until someone requests a conneciton
            if (selector.isReady(listener)){ //incoming connection
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done){
                        clients.push_back(client); //add client to connections
                        std::cout << "New connection received from " << client->getRemoteAddress() << ":" <<
                            client->getRemotePort() << std::endl;

                        selector.add(*client); //add client to selector
                        num_players++;
                        std::cout << "Players connected: " << num_players << std::endl;

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

int ServerCommunicator::broadcast_game_info(Gameinfo info){
    //send each client the game information

    sf::Packet packet;
    for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it){
        sf::TcpSocket& client = **it;








        if(client.send(packet) != sf::Socket::Done){
            std::cout << "Failed to send gameinfo to: " << client.getRemotePort() << std::endl;
        }
    }

    return 0;
};


int ServerCommunicator::accept_inputs(){
    state = 1;
    std::cout << "Now accepting inputs" << std::endl;
    printf("\n");
    selector.remove(listener); //remove listener to reject new connections
    listener.close();

    while(state == 1){
        //iterate through the connections and check for data
        if (clients.size() == 0){
            std::cout << "All clients disconnected" << std::endl;
            return 1;
        }


        selector.wait(); //wait for a socket to be ready

        for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it){

            sf::TcpSocket& client = **it;

            if (selector.isReady(client)){
                // This client has sent some data, we can receive it
                sf::Packet packet;
                sf::Socket::Status status = client.receive(packet);

                if (status == sf::Socket::Done){
                    //extract packet
                    Playerinfo info;

                    packet >> info;

                    std::cout << "Packet recieved from client on port: " << client.getRemotePort() << std::endl;
                    print_playerinfo(info);
                    printf("\n");
                }
                else if(status == sf::Socket::Disconnected){
                    std::cout << "Client has been disconnected" << std::endl;
                    //code to remove client here
                    selector.remove(client);
                    client.disconnect();
                    it = clients.erase(it);

                    std::cout << "Clients remaining: " << clients.size() << std::endl;

                }
                else{
                    std::cout << "Error recieving data from client: " << status << std::endl;
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
