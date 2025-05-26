//this handles all the shooting stuff 

#include "BulletManager.h"
#include <iostream>


//Attempt to fire a bullet if not reloading and cooldown is complete
void BulletManager::tryFire(float x, float y, SDL_Texture* bulletTex) {
  Uint32 currentTime = SDL_GetTicks();
  if (reloading) return;

  //Fire if enough time has passed and bullets remain
  if (currentTime - lastBulletTime >= bulletCooldown && bulletsInMag > 0) {
    // Create a new bullet and add to the list (polymorphic)
    bullets.push_back(std::make_unique<Bullet>(x + 72.0f/2.0f - 8, y, bulletTex));
    bulletsInMag--;
    lastBulletTime = currentTime;

    std::cout << "Bullet fired! Bullets left: " << bulletsInMag << std::endl;

    // Trigger reload if magazine is empty
    if (bulletsInMag == 0) {
         reloading = true;
        reloadStartTime = currentTime;
        std::cout << "Reloading..." << std::endl;
      }
    }
}

//Update all bullets and handle reloading logic
void BulletManager::update() {
    for (auto& bullet : bullets) {
        if (bullet->active) bullet->move();
    }

  //Check if reload duration has passed
  if (reloading) {
    Uint32 now = SDL_GetTicks();
    if (now - reloadStartTime >= reloadDuration) {
        bulletsInMag = maxBullets;
        reloading = false;
        std::cout << "Reload complete." << std::endl;
      }
  }
}

//Draw all active bullets to the screen
void BulletManager::draw(SDL_Renderer* renderer) {
    for (auto& bullet : bullets) {
        if (bullet->active) bullet->draw(renderer);
    }
}
