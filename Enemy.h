#pragma once

#include "Entity.h"
#include <SDL.h>
#include <vector>
#include <memory>
#include "Bullet.h"

class Enemy : public Entity {
    SDL_Texture* texture;
    SDL_Texture* bulletTex;// texture for enemy bullets
    float baseX;
    float waveOffset;

     int shootCooldown; //time between shots (in frames)
     int shootTimer;    //counts up to cooldown

public:
    Enemy(float x, float y, float speed, SDL_Texture* tex, SDL_Texture* bulletTex);

    void move() override;
    void draw(SDL_Renderer* renderer) override;

    void shoot(std::vector<std::unique_ptr<Entity>>& bullets, float playerX, float playerY);

     void setShootCooldown(int cooldown); 
};