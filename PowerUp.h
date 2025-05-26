#pragma once
#include <SDL.h>
#include "Entity.h"
#include <cstdlib>

//possible types of powerups
enum class PowerUpType {
    SpeedBoost,
    MagBoost,
    BulletSpeed
};

//PowerUp class that floats downward and gives boosts
class PowerUp : public Entity {
    SDL_Texture* texture;
    PowerUpType type;
    Uint32 spawnTime;  //when it appeared
    Uint32 duration;   //how long it lasts

public:
    //create a powerup with type and texture
    PowerUp(float x, float y, float speed, SDL_Texture* tex, PowerUpType type);

    void move() override;                      //move down
    void draw(SDL_Renderer* renderer) override; //draw on screen
    PowerUpType getType() const;              //return type

    bool isExpired() const;                   //check if time is up
};