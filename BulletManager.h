#pragma once
#include "Bullet.h"
#include <SDL.h>
#include <iostream>

class BulletManager {
public:
  static const int MAX_BULLETS = 100; //init max bullets
  Bullet bullets[MAX_BULLETS];
  int bulletCount = 0;

  //shooting and reload system
  Uint32 lastBulletTime = 0;
  const Uint32 bulletCooldown = 150;  //setting time in between bullets to 150 ms

  int bulletsInMag = 20;    //20 bullets in each mag
  const int maxBullets = 20;
  bool reloading = false;
  Uint32 reloadStartTime = 0;
  const Uint32 reloadDuration = 1000;   //reload time set to 3 sec


  void tryFire(float x, float y, SDL_Texture* bulletTex);
  void update();
  void draw(SDL_Renderer* renderer);


};