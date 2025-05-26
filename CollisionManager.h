#pragma once

#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PowerUp.h"
#include "AudioManager.h"
#include "EnemyBullet.h"
#include "BulletManager.h"

//handles all game collisions
class CollisionManager {
public:
    //check all collisions in the game
    static void handleCollisions(
    Player& player,
    std::vector<Enemy>& enemies,
    std::vector<std::unique_ptr<Entity>>& bullets,
    std::vector<std::unique_ptr<Entity>>& enemyBullets,
    std::vector<std::unique_ptr<PowerUp>>& powerUps,
    AudioManager& audioManager,
    BulletManager& bulletManager,
    int& score
    );
};