#pragma once
#include <SDL.h>

// defines the Player class, which handles position, movement, and rendering
class Player {
public:
     float x,y;        //player's position on the screen (top-left corner)
     float speed;      //speed at which the player moves
     SDL_Texture* texture;   //texture used to render the player sprite

     Player(float startX, float startY, SDL_Texture* tex); //constructor to initialize position and texture
     void move();    //handles player movement (typically via keyboard input)
     void draw(SDL_Renderer* renderer);  //draws the player sprite to the screen using the renderer
    
};