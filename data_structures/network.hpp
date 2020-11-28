#include <SFML/Network/Packet.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "game.hpp"

#include <list>
#include <vector>

//used for sending position and gunshots from client to server
//also used by server to represent each player to client
struct Playerinfo{ //type 0
    int type;
    std::pair<float, float> position;
    bool moving;
    MovementDirection direction;
    bool bullet_fired;
    sf::Vector2f bullet_direction;
};

struct Playerupdate{
    int type; //type 1
    int playerid;
    float health;
    Playerinfo posinfo;
};

//Distributed by the server to all players minimum 4x a second
struct Gameinfo{
    int type; //type 2
    int success; //If the client failed to recieve a packet, it must set this to 1 so it is ignored
    int num_players; //used so client knows how many playerinfos to expect.
    int client_id; //used so client knows which player in the vector is itself.
    std::vector<Playerupdate> players;
};

struct Gameinitializer{
    int type; //type 3
    std::pair<float, float> spawn_location;
    int client_id;
};

int packet_type(sf::Packet packet);
void print_playerinfo(Playerinfo info);

//includes overloads
//
//overloads for playerinfo
sf::Packet& operator <<(sf::Packet& packet, sf::Vector2f& vec);
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& vec);
sf::Packet& operator <<(sf::Packet& packet, std::pair<float, float>& position);
sf::Packet& operator >>(sf::Packet& packet, std::pair<float, float>& position);
sf::Packet& operator <<(sf::Packet& packet, MovementDirection& direction);
sf::Packet& operator >>(sf::Packet& packet, MovementDirection& direction);
sf::Packet& operator <<(sf::Packet& packet, Playerinfo& info);
sf::Packet& operator >>(sf::Packet& packet, Playerinfo& info);


//overloads for playerupdate
sf::Packet& operator <<(sf::Packet& packet, Playerupdate& info);
sf::Packet& operator >>(sf::Packet& packet, Playerupdate& info);


//overloads for gameinfo
sf::Packet& operator <<(sf::Packet& packet, Gameinfo& info);
sf::Packet& operator >>(sf::Packet& packet, Gameinfo& info);

//overloads for gameinit
sf::Packet& operator <<(sf::Packet& packet, Gameinitializer& info);
sf::Packet& operator >>(sf::Packet& packet, Gameinitializer& info);


void print_metadata(Gameinitializer metadata);
