#pragma once      //ensures this file is only included once during compilation

#include <SDL.h>

//abstrach base classes for all game objects (players, enemies, bullets etc)

class Entity {
  public:
      float x, y;   //position on screen
      float speed;  //movement speed
      bool active = true;

      //constructor that initializes position and speed
      Entity(float x, float y, float speed);

      //all entities must define how they move
      virtual void move() = 0;

      //all entities must define how they draw themselves
      virtual void draw(SDL_Renderer*  renderer) = 0;

      //destructor for cleanup
      virtual ~Entity()  = default;
  
};