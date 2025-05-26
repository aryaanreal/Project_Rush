#pragma once
#include <SDL_mixer.h>

// handles all sound effects and background music
class AudioManager {
    // Sound effects
    Mix_Chunk* fireSound;
    Mix_Chunk* hitSound;
    Mix_Chunk* pickupSound;

    // background music
    Mix_Music* bgm;

public:
    AudioManager();   // constructor
    ~AudioManager();  // destructor

    bool load();          // load all audio assets
    void playFire();      // play fire sound effect
    void playHit();       // play hit sound effect
    void playPickup();    // play power-up pickup sound
    void playMusic();     // start looping background music
    void stopMusic();     // stop background music
};