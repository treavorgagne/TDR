#include <SFML/Audio.hpp>


class SoundPlayer {


    public: 
        SoundPlayer();
        
        void playShoot();
        void playWalk();

    private:

        sf::SoundBuffer shootBuffer;
        sf::SoundBuffer walkBuffer;

        sf::Sound shootSound;
        sf::Sound walkSound; 
         
};