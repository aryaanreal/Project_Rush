#include "EnemyBullet.h"
#include <cmath>

EnemyBullet::EnemyBullet(float x, float y, float speed, SDL_Texture* tex, float targetX, float targetY)
    : Bullet(x, y, tex, speed)
{
    float dirX = targetX - x;
    float dirY = targetY - y;
    float length = std::sqrt(dirX * dirX + dirY * dirY);

    dx = (dirX / length) * speed;
    dy = (dirY / length) * speed;
}

void EnemyBullet::move() {
    x += dx;
    y += dy;

    // deactivate if off-screen
    if (x < 0 || x > 800 || y < 0 || y > 600) {
        active = false;
    }
}

//control bullet size
void EnemyBullet::draw(SDL_Renderer* renderer) {
    SDL_Rect dst = { static_cast<int>(x), static_cast<int>(y), 32, 32 }; 
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
}