#pragma once
#include <SDL.h>

//this class manages level progression every 2 minutes
class LevelGenerator {
    Uint32 startTime;   //time when current level started
    int currentLevel;   //starts at 1 and increases over time

public:
    LevelGenerator();   //constructor

    //call every frame; returns true if level increased
    bool update();

    //get current level
    int getLevel() const;

    //get enemy shooting cooldown based on level (lower = faster shooting)
    int getEnemyShootCooldown() const;
};