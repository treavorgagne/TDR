#include <SFML/System.hpp>

#include "game.hpp"

//used for sending position and gunshots from client to server
struct playerinfo{
    std::pair<int, int> position;
    bool moving;
    MovementDirection direction;
    bool bullet_fired;
    sf::Vector2f bullet_direction;
};
