#include "Enemy.h"
#include "EnemyBullet.h"  //include bullet subclass
#include <cmath>  //for the movement

Enemy::Enemy(float x, float y, float speed, SDL_Texture* tex, SDL_Texture* bulletTex)
  : Entity(x, y, speed), texture(tex), bulletTex(bulletTex), baseX(x),
    shootCooldown(120),                      //set shoot delay (in frames)
    shootTimer(rand() % 120)                 //randomize starting point so enemies don't shoot together
{
    waveOffset = static_cast<float>(rand() % 360); //gives each enemy a unique wave offset
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

  void Enemy::shoot(std::vector<std::unique_ptr<Entity>>& bullets, float playerX, float playerY) {
    shootTimer++; //count frames

    if (shootTimer >= shootCooldown) {
        bullets.push_back(std::make_unique<EnemyBullet>(
            x + 32,       //center of enemy
            y + 84,       //just below enemy sprite
            5.0f,         //bullet speed
            bulletTex,     //texture
            playerX,
            playerY
        ));
        shootTimer = 0; //reset after firing
    }
}