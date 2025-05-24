#include "Player.h"

//constructor sets initial position, speed, and texture
Player::Player(float startX, float startY, SDL_Texture* tex)
    :x(startX), y(startY), speed(5.0f), texture(tex) {}
// handles players movement (to be done later)
void Player::move() {
    //add keyboard logic later
}
//draws the player sprite to the screen
void Player::draw(SDL_Renderer*renderer){
    SDL_Rect dst = { (int)x, (int)y, 64, 64}; //set position and size
    SDL_RenderCopy(renderer, texture, nullptr, &dst); //render the texture
}