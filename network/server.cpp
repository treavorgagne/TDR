
#include "network.hpp"
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <utility>




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
                        std::cout << "Assigned player id: " << num_players << std::endl;

                        selector.add(*client); //add client to selector
                        num_players++;
                        std::cout << "Players connected: " << num_players << std::endl;


                }
                else{
                    std::cout << "Error connecting to a client" << std::endl;
                    listener.close();
                    exit(1);
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

int ServerCommunicator::send_game_metadata(int client_id, float spawnx, float spawny){

    Gameinitializer metadata;

    metadata.client_id = client_id;

    metadata.spawn_location.first = spawnx;
    metadata.spawn_location.second = spawny;

    sf::Packet packet;

    packet << metadata;

    //this is hacky but hopefully sends to the correct client
    std::list<sf::TcpSocket*>::iterator ptr;

    ptr = clients.begin();

    for(int i=0; i < client_id; i++){
        ptr++;
    }


    sf::TcpSocket& client = **ptr;

    client.send(packet);

    return 0;
}

int ServerCommunicator::broadcast_game_info(Gameinfo info){
    //send each client the game information

    sf::Packet packet;
    for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it){
        sf::TcpSocket& client = **it;

        //TODO: BUILT THESE OVERLOADS
        //packet << info;

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
                    std::cout << "Packet recieved from client on port: " << client.getRemotePort() << std::endl;

                    int type = packet_type(packet);
                    std::cout << "packet type is: " << type << std::endl;

                    if(type == 0){
                        Playerinfo info;
                        packet >> info;
                        print_playerinfo(info);
                    }
                    else {
                        std::cout << "Unkown packet type" << std::endl;
                    }


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

