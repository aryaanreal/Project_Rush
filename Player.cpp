#include "Player.h"

//constructor sets initial position, speed, and texture
Player::Player(float startX, float startY, SDL_Texture* tex)
    : Entity(startX, startY, 5.0f), texture(tex) {}     //using base class constructor


//making player movement using wasd and the arrow keys

void Player::handleInput(const Uint8* keystate) {
    if(keystate[SDL_SCANCODE_LEFT]  || keystate[SDL_SCANCODE_A]) {

        x -= speed;     //goes left
    }
    if(keystate[SDL_SCANCODE_RIGHT]  || keystate[SDL_SCANCODE_D]) {

        x += speed;     //goes right
    }
    if(keystate[SDL_SCANCODE_UP]  || keystate[SDL_SCANCODE_W]) {

        y -= speed;     //goes up
    }
    if(keystate[SDL_SCANCODE_DOWN]  || keystate[SDL_SCANCODE_S]) {

        y += speed;     //goes down
    }


    //setting limiters so it doesn't go outside the given play area
    if (x < 0) x = 0;
    if (x > 800 - 64) x = 800-64;    //the player will be 64 bit in x
    if (y < 0) y = 0;
    if (y > 575 - 64) y = 575-64;     //the player will be 64 bit in y too
}

void Player::move() {
    //will add later
}

//player takes dmg
void Player::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

void Player::applyUpgrade(PowerUpType type, BulletManager& BulletManager) {
    switch (type)
    {
    case PowerUpType::SpeedBoost:
        speed += 1.5f;
        break;
    case PowerUpType::MagBoost:
        BulletManager.bulletsInMag += 10;
        break;

    case PowerUpType::BulletSpeed:
        BulletManager.bulletSpeed += 2.0f;
        if(BulletManager.bulletSpeed> 25.0f) BulletManager.bulletSpeed = 25.0f;
    
    break;
    }
}
//draws the player sprite to the screen
void Player::draw(SDL_Renderer*renderer){
    SDL_Rect dst = { (int)x, (int)y, 64,64}; //set position and size
    SDL_RenderCopy(renderer, texture, nullptr, &dst); //render the texture
}