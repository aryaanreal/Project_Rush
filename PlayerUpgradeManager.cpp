#include "PlayerUpgradeManager.h"

#include <string>

void PlayerUpgradeManager::applyUpgrade(Player& player, BulletManager& bulletManager, WaveUpgrade upgrade) {
    currentUpgrade = upgrade;

    switch (upgrade) {
        case WaveUpgrade::HealthRefill:
            player.health = 100;
            break;
        case WaveUpgrade::BulletSpeed:
            bulletManager.bulletSpeed *= 1.5f;
            break;
        case WaveUpgrade::PlayerSpeed:
            player.speed *= 1.5f;
            break;
        default:
            break;
    }
}

void PlayerUpgradeManager::reset(Player& player, BulletManager& bulletManager) {
    if (currentUpgrade == WaveUpgrade::BulletSpeed) {
        bulletManager.bulletSpeed = 10.0f;
    }
    if (currentUpgrade == WaveUpgrade::PlayerSpeed) {
        player.speed = player.baseSpeed;
    }
    currentUpgrade = WaveUpgrade::None;
}

WaveUpgrade PlayerUpgradeManager::getCurrentUpgrade() const {
    return currentUpgrade;
}

// upgrade selection menu
void PlayerUpgradeManager::showUpgradeMenu(Player& player, BulletManager& bulletManager, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Event e;
    int selected = 0;
    bool selecting = true;
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0};

    SDL_StartTextInput();

    while (selecting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w)
                    selected = (selected + 2) % 3;
                if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s)
                    selected = (selected + 1) % 3;
                if (e.key.keysym.sym == SDLK_RETURN) {
                    applyUpgrade(player, bulletManager, static_cast<WaveUpgrade>(selected + 1));
                    selecting = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        std::string options[3] = {
            "Health Refill (Back to 100)",
            "Bullet Speed Boost (x1.5)",
            "Player Speed Boost (x1.5)"
        };

        for (int i = 0; i < 3; ++i) {
            SDL_Color color = (i == selected) ? yellow : white;
            SDL_Surface* surf = TTF_RenderText_Solid(font, options[i].c_str(), color);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_Rect dst = {200, 200 + i * 50, surf->w, surf->h};
            SDL_RenderCopy(renderer, tex, nullptr, &dst);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_StopTextInput();
}
