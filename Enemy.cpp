#include "Enemy.h"
#include <cmath>  //for the movement

Enemy::Enemy(float x, float y, float speed, SDL_Texture* tex)
  : Entity(x, y, speed) , texture(tex), baseX(x) {
    waveOffset = static_cast<float>(rand() % 360); //random waves
 }

 void Enemy::move() {
  //wavy x movememnt and steady downward y movement so the movement doesn't look static
  float waveAmplitude = 40.0f;
  float wavespeeed = 0.05f;

  x = baseX + std::sin((y + waveOffset) * wavespeeed) * waveAmplitude;
  y += speed;

  if (y> 600) active = false;

 }

 void Enemy::draw(SDL_Renderer* renderer) {
    SDL_Rect dst = { static_cast<int>(x), static_cast<int>(y), 84, 84 };
    SDL_RenderCopy(renderer, texture, nullptr, &dst); 
  }
 