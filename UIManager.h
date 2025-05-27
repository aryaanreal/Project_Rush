//header file for ui manager that will now hold all ui elements that wass in main
#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "PowerUp.h"
#include "scoremanager.h"
#include "PlayerUpgradeManager.h"

class UIManager {
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color color;

public:
    UIManager(SDL_Renderer* renderer, TTF_Font* font);
    void drawHUD(int bulletsLeft, bool reloading, int health, int score, PowerUpType currentPowerUp, int wave, bool hasPowerUp);
    
    void drawText(const std::string& text, int x, int y);
    enum class MenuOption { Start, Quit };
    void drawStartMenu(MenuOption selected);
    enum class PauseOption { Continue, Quit };
    void drawPauseMenu(PauseOption selected);
    void drawGameOverScreen(MenuOption selected);
    void drawleaderboard(const std::vector<scoreentry>& scores);
    std::string getinitials();
    void drawWaveUpgrade(WaveUpgrade upgrade);



    
};
