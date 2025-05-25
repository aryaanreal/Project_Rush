#pragma once
#include "Entity.h"
#include <SDL.h>

//this is the enemy class the 'space invaders'

class Enemy : public Entity {
  SDL_Texture* texture;
  float baseX;   //original x to move around
  float waveOffset;  //phase offset to makee the movement unique

public:
  Enemy(float x, float y, float speed, SDL_Texture* tex);
  void move() override;
  void draw(SDL_Renderer* renderer) override;

};