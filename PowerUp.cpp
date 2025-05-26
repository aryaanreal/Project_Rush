#include "PowerUp.h"

//constructor sets position, texture, type, and random duration
PowerUp::PowerUp(float x, float y, float speed, SDL_Texture* tex, PowerUpType type)
    : Entity(x, y, speed), texture(tex), type(type) {
    spawnTime = SDL_GetTicks();
    duration = 5000 + rand() % 3000; // 5 to 8 seconds
}

//move powerup down the screen
void PowerUp::move() {
    y += speed;
    if (y > 600) active = false; // deactivate if it leaves the screen
}

//draw powerup to screen
void PowerUp::draw(SDL_Renderer* renderer) {
    SDL_Rect dst = { static_cast<int>(x), static_cast<int>(y), 32, 32 };
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
}

//return the type of powerup (for effect)
PowerUpType PowerUp::getType() const {
    return type;
}

//check if powerup expired
bool PowerUp::isExpired() const {
    return (SDL_GetTicks() - spawnTime) >= duration;
}