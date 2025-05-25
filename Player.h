#pragma once
#include <SDL.h>
#include "Entity.h"

// defines the Player class, which handles position, movement, and rendering
class Player : public Entity {
public:
 
     SDL_Texture* texture;   //texture used to render the player sprite

     void handleInput(const Uint8* keystate);     //keyboard inputs are noww in
     Player(float startX, float startY, SDL_Texture* tex); //constructor to initialize position and texture
     void move() override;    //handles player movement (typically via keyboard input)
     void draw(SDL_Renderer* renderer) override;  //draws the player sprite to the screen using the renderer
    
};