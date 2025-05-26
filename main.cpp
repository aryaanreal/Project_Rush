//making core sdl functionality
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <memory>


#include "Player.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "Enemy.h"
#include "AudioManager.h"
#include "LevelGenerator.h"
#include "PowerUp.h"
#include "CollisionManager.h"

//setting up the screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
  //initializing SDL video system
  if(SDL_Init(SDL_INIT_VIDEO) <  0) {
    std::cerr << "SDL couldn't initialize" << SDL_GetError() << "\n"; //outputting if there is an error
    return 1;
  }

//initializing SDL_mixer
if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
  std::cerr << "SDL_mixer couldn't be initialized. SDL_mixer error: " << Mix_GetError() << "\n";
  return 1;
}

//create and load audio manager
AudioManager audio;
if(!audio.load()) {
  std::cerr << "Failed to load audio" << "\n";
  return 1;
}

//initializing sdl_ttf
if(TTF_Init() < 0) {
  std::cerr << "Failed to initialize SDL_TTF" << SDL_GetError << "\n";
  return 1;
}

//load fonts
TTF_Font* font = TTF_OpenFont("assets/EOG.ttf", 24);
if(!font) {
  std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
  return 1;
}

//seed rng for enemy generation

srand(static_cast<unsigned int>(time(nullptr))); 

//create a window in the center of the screen

SDL_Window* window = SDL_CreateWindow("Galaxy Rush",
  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);


//checking if the window creatuin failed or not
if (!window) {
  //using cerr to display error messages
  std::cerr << "Window couldn't be created!" << SDL_GetError() << "\n";
  return 1;
}

//creating renderer to show sprites on screen
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
if(!renderer) {
  std::cerr << "renderer dould not be created" << SDL_GetError() << "\n";
  return 1;
}

//initializing SDL_image to load png files from assets
if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
  std::cerr <<"Failed to init SDL_image" << "\n";
  return 1;
}

//load the player sprite
SDL_Surface* surface = IMG_Load("assets/player.png");
if(!surface) {
  std::cerr << "Failed to load image" << IMG_GetError() << "\n";
  return 1;
}

//creating the texture for the surface
SDL_Texture* playerTex = SDL_CreateTextureFromSurface(renderer, surface);
SDL_FreeSurface(surface);   //surface no longer needed after texture is made


//load bullet sprite
SDL_Surface* bulletSurface = IMG_Load("assets/bullet.png");
if(!bulletSurface) {
  std::cerr << "Failed to load bullet image: " << IMG_GetError() << "\n";
  return 1;
}

//loading enemy bullet sprites (the same as the player)
SDL_Surface* bulletSurf = IMG_Load("assets/enemyBullet.png");
SDL_Texture* enemyBulletTex = SDL_CreateTextureFromSurface(renderer, bulletSurf);


//creating the bullet surface
SDL_Texture* bulletTex = SDL_CreateTextureFromSurface(renderer, bulletSurface);
SDL_FreeSurface(bulletSurface);


//loading enemy sprites
SDL_Surface* enemySurface = IMG_Load("assets/enemy.png"); 
if (!enemySurface) {
  std::cerr << "Failed to load enemy image: " << IMG_GetError() << "\n";
  return 1;
}

//loading powerups sprite
SDL_Surface* powerupSurface = IMG_Load("assets/powerup.png");
if (!powerupSurface) {
    std::cerr << "Failed to load powerup image: " << IMG_GetError() << "\n";
    return 1;
}
SDL_Texture* powerupTex = SDL_CreateTextureFromSurface(renderer, powerupSurface);
SDL_FreeSurface(powerupSurface);

if (!powerupTex) {
    std::cerr << "Failed to create powerup texture: " << SDL_GetError() << "\n";
    return 1;
}

//start background music
audio.playMusic();


SDL_Texture* enemyTex = SDL_CreateTextureFromSurface(renderer, enemySurface);
SDL_FreeSurface(enemySurface);

//loading background
SDL_Surface* bgSurface = IMG_Load("assets/background.png");
if (!bgSurface) {
  std::cerr << "Failed to load bg: " << IMG_GetError() << "\n";
  return 1;
}

SDL_Texture* backgroundTex = SDL_CreateTextureFromSurface(renderer, bgSurface);
SDL_FreeSurface(bgSurface);


//game loop control variable
bool isRunning = true;

//SDL event variables to track key presses, quitting and stuff
SDL_Event event;

//PLayer's position on the screen

Player player(400, 500, playerTex);     //using the player start position func inherited from entity
BulletManager bulletManager;  //calling bullet manager


//calling the enemiess
std::vector<Enemy> enemies;
Uint32 lastSpawnTime = 0;
std::vector<std::unique_ptr<Entity>> enemyBullets;

//setting up the powerups
std::vector<std::unique_ptr<PowerUp>> powerUps; 
//Uint32 lastSpawnTime = 0; commented cuz already declared
Uint32 lastPowerUpTime = 0;

//keeeeping track of scooree
int score = 0;
LevelGenerator levelGen;

//setting up main game loop now
while (isRunning) {
  //poll for any events like clsoing the window or key presses
 
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) isRunning = false;
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        bulletManager.tryFire(player.x, player.y, bulletTex);
        audio.playFire();
        }
    }


  //getting keystate and pass to the player's input
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  player.handleInput(keystate);


  //check for player health
  if (player.health <= 0) {
    std::cout << "Game Over!\n";
    isRunning = false;
    continue;
  }


  bulletManager.update();   //updates bullet manager
  


  for (auto& bullet : enemyBullets) {
    if (bullet->active) bullet->move();
}

//level up every 2 minutes
if (levelGen.update()) {
    std::cout << "Level up! Now at level " << levelGen.getLevel() << std::endl;
}

  //making it so new enemies spawn
  Uint32 currentTime = SDL_GetTicks();
  if (currentTime - lastSpawnTime > 1000) {
    float xPos = rand() % (SCREEN_WIDTH -48);
    enemies.emplace_back(xPos, -48, 2.0f, enemyTex, enemyBulletTex);
    enemies.back().setShootCooldown(levelGen.getEnemyShootCooldown()); //apply cooldown
    lastSpawnTime = currentTime;
  }

  //move enemies
  for (auto& enemy: enemies) {
    if(enemy.active) enemy.move(); 
  }

  for (auto& enemy : enemies) {
    if (enemy.active) {
        enemy.shoot(enemyBullets, player.x, player.y); //enemy fires with cooldown
    }
}

  //spawning in the powerups
  if (currentTime - lastPowerUpTime > 10000) {
    float x = rand() % (SCREEN_WIDTH - 32);
    PowerUpType type = static_cast<PowerUpType>(rand() % 3);
    powerUps.push_back(std::make_unique<PowerUp>(x, -32, 2.0f, powerupTex, type));
    lastPowerUpTime = currentTime;
  }

  for (auto& enemy: enemies) if (enemy.active) enemy.move();
  for (auto& pu : powerUps) if (pu -> active) pu->move();       //basically makes the enemy move if it spawns on a powerup

  //handling collisions
  CollisionManager::handleCollisions(player, enemies, bulletManager.bullets, enemyBullets,  powerUps, audio, bulletManager, score);


  //drawing bg image
  SDL_RenderCopy(renderer, backgroundTex, nullptr, nullptr);

  //drawing the player texture 
  player.draw(renderer);

  //drawing bullets
  bulletManager.draw(renderer);

  //drawing the powerups
  for (auto& pu : powerUps) {
    if (pu->active) pu->draw(renderer);
  }


  //drawing enemies
  for(auto& enemy : enemies) {
    if(enemy.active) enemy.draw(renderer);
  }

  //draw enemy bullets 
  for (auto& bullet : enemyBullets) {
  if (bullet->active) bullet->draw(renderer);
}

  //UI ELEMENTS (Will be transfered to ui manager later)

  SDL_Color white = {255, 255, 255};    //setting the text color white

  SDL_Surface* labelSurface = TTF_RenderText_Blended(font, "Bullets", white);     //it wwill say bullet on top
  SDL_Texture* labelTex = SDL_CreateTextureFromSurface(renderer, labelSurface);
  SDL_Rect labelRect = {10, SCREEN_HEIGHT - 50, labelSurface->w,labelSurface->h};   //the position
  SDL_RenderCopy(renderer, labelTex, nullptr, &labelRect);
  SDL_FreeSurface(labelSurface);
  SDL_DestroyTexture(labelTex);

  std::string bulletStatus = bulletManager.reloading ? "Reloading" : std::to_string(bulletManager.bulletsInMag);    //it will check if bulletmanager says reloading if it does
  SDL_Surface* statusSurface = TTF_RenderText_Blended(font, bulletStatus.c_str(), white);                           // it will show reloading
  SDL_Texture* statusTex = SDL_CreateTextureFromSurface(renderer, statusSurface);
  SDL_Rect statusRect = {10, SCREEN_HEIGHT - 25, statusSurface->w, statusSurface->h};
  SDL_RenderCopy(renderer, statusTex, nullptr, &statusRect);
  SDL_FreeSurface(statusSurface);
  SDL_DestroyTexture(statusTex);

  //show hp
  std::string hpText = "HP: " + std::to_string(player.health);
  SDL_Surface* hpSurface = TTF_RenderText_Blended(font, hpText.c_str(), white);
  SDL_Texture* hpTex = SDL_CreateTextureFromSurface(renderer, hpSurface);
  SDL_Rect hpRect = { SCREEN_WIDTH - 120, SCREEN_HEIGHT - 50, hpSurface->w, hpSurface->h };
  SDL_RenderCopy(renderer, hpTex, nullptr, &hpRect);
  SDL_FreeSurface(hpSurface);
  SDL_DestroyTexture(hpTex);

  //shoow score
  std::string scoreText = "Score: " + std::to_string(score);
  SDL_Surface* scoreSurface = TTF_RenderText_Blended(font, scoreText.c_str(), white);
  SDL_Texture* scoreTex = SDL_CreateTextureFromSurface(renderer, scoreSurface);
  SDL_Rect scoreRect = { SCREEN_WIDTH / 2 - 50, 10, scoreSurface->w, scoreSurface->h };
  SDL_RenderCopy(renderer, scoreTex, nullptr, &scoreRect);
  SDL_FreeSurface(scoreSurface);
  SDL_DestroyTexture(scoreTex);
  

  

  //showing everything we drew
  SDL_RenderPresent(renderer);

  //delay to make framerate (60 fps)
  SDL_Delay(16);


}

//clean up all sdl resources to save computing power
SDL_DestroyTexture(bulletTex);
SDL_DestroyTexture(playerTex);
SDL_DestroyRenderer(renderer);
SDL_DestroyTexture(enemyTex);
TTF_CloseFont(font);
TTF_Quit();
Mix_CloseAudio();
SDL_DestroyWindow(window);
IMG_Quit();
SDL_Quit();

return 0; //exit program

}