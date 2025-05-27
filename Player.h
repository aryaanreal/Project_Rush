#pragma once
#include <SDL.h>
#include "Entity.h"
#include "PowerUp.h"
#include "BulletManager.h"


// defines the Player class, which handles position, movement, and rendering
class Player : public Entity {
public:
     
     float baseSpeed = 5.0f; //for restoring after temporary boost

     SDL_Texture* texture;   //texture used to render the player sprite
     int health = 100;        //player health

     void handleInput(const Uint8* keystate);     //keyboard inputs are noww in
     Player(float startX, float startY, SDL_Texture* tex); //constructor to initialize position and texture
     void move() override;    //handles player movement (typically via keyboard input)
     void draw(SDL_Renderer* renderer) override;  //draws the player sprite to the screen using the renderer

     void takeDamage(int dmg); //player taking damage
     void applyUpgrade(PowerUpType type, BulletManager& BulletManager);
    
};