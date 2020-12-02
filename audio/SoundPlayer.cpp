#include <SFML/Audio.hpp>
#include "SoundPlayer.hpp"

using namespace sf;



SoundPlayer::SoundPlayer(){

    shootBuffer.loadFromFile("../media/shot.wav");
    walkBuffer.loadFromFile("../media/wall.wav");
    
    shootSound.setBuffer(shootBuffer);
    walkSound.setBuffer(walkBuffer);

}


void SoundPlayer::playWalk(){
    walkSound.play();
}

void SoundPlayer::playShoot(){
    shootSound.play();
}


