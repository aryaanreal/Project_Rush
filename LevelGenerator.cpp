#include "LevelGenerator.h"

LevelGenerator::LevelGenerator() : currentLevel(1) {
    startTime = SDL_GetTicks();  //record game start time
}

//check if it's time to level up 
bool LevelGenerator::update() {
    Uint32 now = SDL_GetTicks();
    if ((now - startTime) >= 10000) {
        currentLevel++;      //increase level
        startTime = now;     //reset timer
        return true;         //notify game that level changed
    }
    return false;
}

//return current level number
int LevelGenerator::getLevel() const {
    return currentLevel;
}

//return enemy shoot cooldown (decreases as level increases)
int LevelGenerator::getEnemyShootCooldown() const {
    int base = 120;                         //base cooldown in frames
    int reduction = (currentLevel - 1) * 10; //cooldown decreases by 10 each level
    return base > reduction ? (base - reduction) : 30; //minimum cooldown = 30
}