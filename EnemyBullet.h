#pragma once
#include "Bullet.h"

//enemyBullet inherits from bullet and uses the same behavior
class EnemyBullet : public Bullet {
public:
    //constructor calls base bullet constructor with speed and texture
    EnemyBullet(float x, float y, float speed, SDL_Texture* tex)
        : Bullet(x, y, tex, speed) {}
};