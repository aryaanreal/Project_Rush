#include "Bullet.h" //include the header file for the Bullet class

Bullet::Bullet()
    : Entity(0,0,0), texture(nullptr), active(false) {}     //adding a default constructor so the compiler knows how to make a bullet

//Constructor: sets the position, speed, and texture by calling the Entity constructor
Bullet::Bullet(float x, float y, SDL_Texture*tex, float speed)
     :Entity(x,y, speed), texture (tex), active(true) {}

//moves the bullet upward by decreasing its y position
void Bullet::move(){
    y-= speed;
    if (y < -32) active = false; //cheeck if the bullet is outside the play area
}

//draws the bullet upward by decreasing its y position
void Bullet::draw(SDL_Renderer* renderer) {
    //create a bullet using the given sprite
    SDL_Rect dst = { static_cast<int>(x), static_cast<int>(y), 8, 20 };  //adjust size to image

    
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
}