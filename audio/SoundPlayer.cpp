
#include "SoundPlayer.hpp"




SoundPlayer::SoundPlayer() {

    shootBuffer.loadFromFile("shot.wav");
    walkBuffer.loadFromFile("wall.wav");
    
    shootSound.setBuffer(shootBuffer);
    walkSound.setBuffer(walkBuffer);

}

void SoundPlayer::playShoot(){
    shootSound.play();
}

void SoundPlayer::playWalk(){
    walkSound.play();
}

