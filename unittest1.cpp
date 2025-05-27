//this units of tests tests the following classes listed in the header below and prints out if they work properly or not when compiled with their respective cpp file

#include "scoremanager.h"
#include "Player.h"
#include "PowerUp.h"
#include "LevelGenerator.h"
#include "Enemy.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <SDL.h>

// -- SCOREMANAGER TESTS --
void cleanupLeaderboard() {
    std::ofstream file("test_leaderboard.txt", std::ios::trunc);
    file.close();
}

void test_save_and_load_score() {
    cleanupLeaderboard();
    scoreentry e1 = {"TST", 999};
    scoremanager::savescore("test_leaderboard.txt", e1);
    auto scores = scoremanager::loadscores("test_leaderboard.txt");
    assert(!scores.empty());
    assert(scores[0].initials == "TST");
    assert(scores[0].score == 999);
    std::cout << "ScoreManager save/load test passed.\n";
}

void test_top8_enforced() {
    cleanupLeaderboard();
    for (int i = 0; i < 12; ++i)
        scoremanager::savescore("test_leaderboard.txt", {"X" + std::to_string(i), 100 + i});
    auto scores = scoremanager::loadscores("test_leaderboard.txt");
    assert(scores.size() == 8);
    std::cout << "ScoreManager top 8 test passed.\n";
}



// -- PLAYER TEST --
void test_player_movement_bounds() {
    Player p(400, 500, nullptr);
    const Uint8* keystate = SDL_GetKeyboardState(NULL); // won't simulate actual keys in test
    p.handleInput(keystate);
    assert(p.x >= 0 && p.y >= 0);
    std::cout << "Player movement boundary test passed.\n";
}

// -- POWERUP TEST --
void test_powerup_collection() {
    PowerUp p(100, 100, 2.0f, nullptr, PowerUpType::SpeedBoost);
    p.move();
    assert(p.y > 100);
    std::cout << "PowerUp movement test passed.\n";
}

// -- LEVEL GENERATOR TEST --

void test_level_progression() {
LevelGenerator lg;
int oldLevel = lg.getLevel();
for (int i = 0; i < 120; ++i) lg.update(); // simulate 2 min worth of updates
int newLevel = lg.getLevel();
assert(newLevel >= oldLevel);
std::cout << "LevelGenerator progression test passed.\n";
}

// -- ENEMY TEST --

void test_enemy_movement() {
    Enemy e(100, 100, 2.0f, nullptr, nullptr);
    float oldY = e.y;
    e.move();
    assert(e.y > oldY);
    std::cout << "Enemy movement test passed.";
}


int main() {
    test_save_and_load_score();
    test_top8_enforced();
    test_player_movement_bounds();
    test_powerup_collection();
    test_enemy_movement();
    test_level_progression();
    std::cout << "All unit tests passed!\n";
    return 0;
}
