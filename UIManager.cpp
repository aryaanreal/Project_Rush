#include "UIManager.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include "scoremanager.h"
#include "PlayerUpgradeManager.h"
#include <cctype>

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
  drawText("Entering Leaderboards", 330, 240);

  color = (selected == MenuOption::Quit ? SDL_Color{255, 255, 0} : original);
  drawText("Nice Try", 340, 290);

  color = original;
  SDL_RenderPresent(renderer);
}

//leaderboard
void UIManager::drawleaderboard(const std::vector<scoreentry>& scores) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  drawText("leaderboard", 300, 50);

  int y = 120;
  for (const auto& entry : scores) {
    drawText(entry.initials + " - " + std::to_string(entry.score), 300, y);
    y += 40;
  }

  drawText("press esc to return", 270, y + 40);
  SDL_RenderPresent(renderer);
}

//getting the initials for the leaderboard
std::string UIManager::getinitials() {
  std::string initials = "";
  SDL_StartTextInput();
  SDL_Event e;
  
  while (initials.length() < 3) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) return "";
      if (e.type == SDL_TEXTINPUT) {
        char inputChar = e.text.text[0];
        if (std::isalpha(static_cast<unsigned char>(inputChar))) {
          initials += std::toupper(static_cast<unsigned char>(inputChar));
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Color color = {255, 255, 255};
    std::string display = "Enter Initials: " + initials;
    SDL_Surface* surface = TTF_RenderText_Solid(font, display.c_str(), color);
    if (surface) {
      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_Rect dst = {200, 250, surface->w, surface->h};
      SDL_RenderCopy(renderer, texture, nullptr, &dst);
      SDL_FreeSurface(surface);
      SDL_DestroyTexture(texture);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }
  SDL_StopTextInput();
  return initials;
}

void UIManager::drawWaveUpgrade(WaveUpgrade upgrade) {
    std::string upgradeText = "Wave Upgrade: ";

    switch (upgrade) {
        case WaveUpgrade::HealthRefill:
            upgradeText += "Health Refill";
            break;
        case WaveUpgrade::BulletSpeed:
            upgradeText += "Bullet Speed";
            break;
        case WaveUpgrade::PlayerSpeed:
            upgradeText += "Player Speed";
            break;
        default:
            upgradeText += "None";
    }

    // draw it safely in the bottom-left corner under HUD
    drawText(upgradeText, 10, 520);
}

