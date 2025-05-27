#pragma once
#include "Player.h"
#include "BulletManager.h"
#include "SDL_ttf.h"
#include "SDL.h"

enum class WaveUpgrade {
    None,
    HealthRefill,       //these are all the upgrades
    BulletSpeed,
    PlayerSpeed
};

class PlayerUpgradeManager {
    WaveUpgrade currentUpgrade = WaveUpgrade::None;

public:
    void showUpgradeMenu(Player& player, BulletManager& bulletManager, SDL_Renderer* renderer, TTF_Font* font);

    void applyUpgrade(Player& player, BulletManager& bulletManager, WaveUpgrade upgrade);

    void reset(Player& player, BulletManager& bulletManager);

    WaveUpgrade getCurrentUpgrade() const;
};
