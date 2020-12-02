#include <SFML/Audio.hpp>

using namespace sf;

class SoundPlayer {

    private:
        SoundBuffer ShootBuffer;
        SoundBuffer WalkingBuffer;

        Sound shootSound;
        Sound walkSound;
    
    public: 
        SoundPlayer();
        void playWalk();
        void playShoot();
};