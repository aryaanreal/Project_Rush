#include "Bullet.h" //include the header file for the Bullet class

//Constructor: sets the position, speed, and texture by calling the Entity constructor
Bullet::Bullet(float x, float y, float speed, SDL_Texture*tex)
     :Entity(x,y, speed), texture (tex) {}

//moves the bullet upward by decreasing its y position
void Bullet::move(){
    y-= speed;
}

//draws the bullet upward by decreasing its y position
void Bullet::draw(SDL_Renderer* renderer) {
    // create a rectangle where the bullet will be drawn
    SDL_Rect dst = { static_cast<int>(x), static_cast<int>(y), 16, 32 };

    // render the texture onto the screen at the given rectangle
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
}