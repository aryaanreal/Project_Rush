#pragma once
#include "Bullet.h"

//enemyBullet inherits from bullet and uses the same behavior
class EnemyBullet : public Bullet {
    //dirrection vectors so the bullet points at the player
    float dx, dy;
public:
    //constructor calls base bullet constructor with speed and texture
    EnemyBullet(float x, float y, float speed, SDL_Texture* tex, float targetX, float targetY);
    void move() override;
};