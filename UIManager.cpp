#include "UIManager.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>

UIManager::UIManager(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font), color({255, 255, 255}) {}   //font coolor



void UIManager::drawText(const std::string& text, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
//shooting /reloading font
void UIManager::drawHUD(int bulletsLeft, bool reloading, int health, int score, PowerUpType currentPowerUp, int wave, bool hasPowerUp) {
    drawText("Bullets: " + (reloading ? std::string("Reloading") : std::to_string(bulletsLeft)), 10, 550);
    drawText("HP: " + std::to_string(health), 680, 550);
    drawText("Score: " + std::to_string(score), 360, 10);

//showing the powerup
    std::string pu = (hasPowerUp ? (
    currentPowerUp == PowerUpType::SpeedBoost ? "Speed" :
    currentPowerUp == PowerUpType::MagBoost ? "Magazine" :
    currentPowerUp == PowerUpType::BulletSpeed ? "BulletSpeed" : "None"
    ) : "None");

    drawText("PowerUp: " + pu, 10, 10);
    drawText("Wave: " + std::to_string(wave), 680, 10);   //showing wave number
}

void UIManager::drawStartScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    drawText("GALAXY RUSH", 240, 200);
    drawText("Press ENTER to Play", 290, 300);
    drawText("Press ESC to Exit", 290, 340);
    SDL_RenderPresent(renderer);
}