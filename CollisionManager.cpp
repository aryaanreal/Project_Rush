#include "CollisionManager.h"

//check if two rectangles overlap (simple AABB)
static bool checkCollision(float x1, float y1, float w1, float h1,
                           float x2, float y2, float w2, float h2) {
    return (x1 < x2 + w2) && (x1 + w1 > x2) &&
           (y1 < y2 + h2) && (y1 + h1 > y2);
}

void CollisionManager::handleCollisions(
    Player& player,
    std::vector<Enemy>& enemies,
    std::vector<std::unique_ptr<Entity>>& bullets,
    std::vector<std::unique_ptr<PowerUp>>& powerUps,
    AudioManager& audioManager,
    BulletManager& bulletmanager,
    int& score
) {
    //player bullets vs. enemies
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        bool hit = false;

        for (Enemy& enemy : enemies) {
            if (!enemy.active) continue;

            if (checkCollision(
                (*bulletIt)->x, (*bulletIt)->y, 16, 32,
                enemy.x, enemy.y, 64, 64
            )) {
                audioManager.playHit();
                enemy.active = false; 
                score += 100;
                hit = true;
                break;
            }
        }
        if (hit) {
            bulletIt = bullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }

    //enemy bullets vs. player
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        EnemyBullet* eBullet = dynamic_cast<EnemyBullet*>(bulletIt->get());
        if (eBullet && checkCollision(
            eBullet->x, eBullet->y, 16, 32,
            player.x, player.y, 64, 64
        )) {
            player.takeDamage(10);
            audioManager.playHit();
            bulletIt = bullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }

    //powerUps vs. player
    for (auto puIt = powerUps.begin(); puIt != powerUps.end(); ) {
        if (checkCollision(
            player.x, player.y, 64, 64,
            (*puIt)->x, (*puIt)->y, 32, 32
        )) {
            player.applyUpgrade((*puIt)->getType(), bulletmanager);
            audioManager.playPickup();
            puIt = powerUps.erase(puIt);
        } else {
            ++puIt;
        }
    }
}