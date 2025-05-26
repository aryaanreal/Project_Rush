#pragma once
#include <SDL.h>
#include <vector>
#include <memory>
#include "Bullet.h"
#include "Entity.h"

class BulletManager {
public:
    std::vector<std::unique_ptr<Entity>> bullets;  //polymorphic storage

    int bulletsInMag = 20;
    int maxBullets = 20;
    bool reloading = false;
    Uint32 reloadDuration = 2000; 
    Uint32 reloadStartTime = 0;
    Uint32 lastBulletTime = 0;
    Uint32 bulletCooldown = 250; 

    float bulletSpeed = 10.0f; 

    void tryFire(float x, float y, SDL_Texture* bulletTex);
    void update();
    void draw(SDL_Renderer* renderer);
};
