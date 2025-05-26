#include "AudioManager.h"
// initialize all sound pointers to nullptr
AudioManager::AudioManager()
    : fireSound(nullptr), hitSound(nullptr), pickupSound(nullptr), bgm(nullptr) {}
// free all loaded sound effects and music from memory
 AudioManager::~AudioManager() {
    Mix_FreeChunk(fireSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(pickupSound);
    Mix_FreeMusic(bgm);
}

//load all audio files from the assets folder
bool AudioManager::load() {
    fireSound = Mix_LoadWAV("assets/fire.wav");
    hitSound = Mix_LoadWAV("assets/hit.wav");
    pickupSound = Mix_LoadWAV("assets/pickup.wav");
    bgm = Mix_LoadMUS("assets/background_music.mp3");


//return true only if all sounds are succesfull
return fireSound && hitSound && pickupSound && bgm;
}

//to play the fire sound effect
void AudioManager::playFire() {
    if (fireSound) Mix_PlayChannel(-1, fireSound, 0);
}

//to play the hit sound effect
void AudioManager::playHit() {
        if (hitSound) Mix_PlayChannel(-1, hitSound, 0);
}

//to play the power-up pickup sound effect
void AudioManager::playPickup() {
    if (pickupSound) Mix_PlayChannel(-1, pickupSound, 0);
}

//start looping the background music
void AudioManager::playMusic() {
    if (bgm) Mix_PlayMusic(bgm, -1);
}

//to stop the bgm
void AudioManager::stopMusic() {
    Mix_HaltMusic();
}