//this handles all the shooting stuff 

#include "BulletManager.h"

void BulletManager::tryFire(float x, float y, SDL_Texture* bulletTex) {
  Uint32 currentTime = SDL_GetTicks();
  if (reloading) return;

  if (currentTime - lastBulletTime >= bulletCooldown && bulletsInMag > 0) {
    if (bulletCount < MAX_BULLETS){
      bullets[bulletCount++] = Bullet(x + 24.0f, y, bulletTex);
      if(bulletCount >= MAX_BULLETS) {
        bulletCount = 0; //loop back to 100
      }
      bulletsInMag--;
      lastBulletTime = currentTime;
      std::cout << "Bullets fired! Left: " << bulletsInMag << "\n"; //keeping track in terminal

      if(bulletsInMag == 0) {
        reloading = true;
        reloadStartTime = currentTime;
        std::cout << "Reloading..." << '\n';
      }
    }
  }
}

//reloading shenanigans
void BulletManager::update() {
  for (int i = 0; i < bulletCount; i++){
    if (bullets[i].active) bullets[i].move();
  }
  
  if (reloading) {
    Uint32 now = SDL_GetTicks();
    if (now - reloadStartTime >= reloadDuration) {
      bulletsInMag = maxBullets;
      reloading = false;
      std::cout << "Reloading complete crush them!" << "\n";
    }
  }
}

void BulletManager::draw(SDL_Renderer* renderer) {
  for (int i = 0; i < bulletCount; i++){
    if (bullets[i].active) bullets[i].draw(renderer);
  }
}