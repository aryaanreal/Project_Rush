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

void UIManager::drawStartMenu(MenuOption selected) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawText("GALAXY RUSH", 240, 150);

    SDL_Color original = color;
    color = (selected == MenuOption::Start ? SDL_Color{255, 255, 0} : original);
    drawText("Start", 340, 250);

    color = (selected == MenuOption::Quit ? SDL_Color{255, 255, 0} : original);
    drawText("Quit", 340, 300);

    color = original;
    SDL_RenderPresent(renderer);
}

//adding a pause menu
void UIManager::drawPauseMenu(PauseOption selected) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // semi-transparent black
  SDL_Rect overlay = {200, 180, 400, 240};
  SDL_RenderFillRect(renderer, &overlay);

  SDL_Color original = color;
  color = (selected == PauseOption::Continue ? SDL_Color{255, 255, 0} : original);
  drawText("Continue", 320, 220);

  color = (selected == PauseOption::Quit ? SDL_Color{255, 255, 0} : original);
  drawText("Quit", 340, 280);

  color = original;
  SDL_RenderPresent(renderer);
}

//game over screen
void UIManager::drawGameOverScreen(MenuOption selected) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  drawText("GAME OVER", 280, 150);

  SDL_Color original = color;
  color = (selected == MenuOption::Start ? SDL_Color{255, 255, 0} : original);
  drawText("Restart", 330, 240);

  color = (selected == MenuOption::Quit ? SDL_Color{255, 255, 0} : original);
  drawText("Quit", 340, 290);

  color = original;
  SDL_RenderPresent(renderer);
}