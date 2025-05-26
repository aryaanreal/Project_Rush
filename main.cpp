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
#include "UIManager.h"

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

//load uimanager
UIManager ui(renderer, font);


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



//startup scrreen!
UIManager::MenuOption selected = UIManager::MenuOption::Start;

bool menuRunning = true;

while (menuRunning) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return 0;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
                selected = (selected == UIManager::MenuOption::Start) ? UIManager::MenuOption::Quit : UIManager::MenuOption::Start;
            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
                selected = (selected == UIManager::MenuOption::Quit) ? UIManager::MenuOption::Start : UIManager::MenuOption::Quit;

            if (event.key.keysym.sym == SDLK_RETURN) {
                if (selected == UIManager::MenuOption::Start)
                    menuRunning = false;
                  else
                      return 0;
            }
        }
    }

        ui.drawStartMenu(selected);
        SDL_Delay(16);
}

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

//keeping track of the last powerup
PowerUpType lastCollectedPowerUp = PowerUpType::SpeedBoost;
bool hasActivePowerup = false;    //adding new check to see if it actually has a powerup or nah

//keeeeping track of scooree
int score = 0;
LevelGenerator levelGen;

//setting up main game loop now
while (isRunning) {
  //poll for any events like clsoing the window or key presses
 
while (isRunning) {
  while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) isRunning = false;
      else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
          bulletManager.tryFire(player.x, player.y, IMG_LoadTexture(renderer, "assets/bullet.png"));
          audio.playFire();
      } else if (event.key.keysym.sym == SDLK_ESCAPE) {
          UIManager::PauseOption pauseSelected = UIManager::PauseOption::Continue;
          bool paused = true;

          //addedd logic for pausing the game
          while (paused) {
              while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) return 0;
                  if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
                          pauseSelected = (pauseSelected == UIManager::PauseOption::Continue) ? UIManager::PauseOption::Quit : UIManager::PauseOption::Continue;
                      if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
                          pauseSelected = (pauseSelected == UIManager::PauseOption::Quit) ? UIManager::PauseOption::Continue : UIManager::PauseOption::Quit;

                      if (event.key.keysym.sym == SDLK_RETURN) {
                          if (pauseSelected == UIManager::PauseOption::Continue)
                              paused = false;
                          else
                              return 0;
                      }
                  }
              }

              ui.drawPauseMenu(pauseSelected);
              SDL_Delay(16);
            }
        }
    }
  //getting keystate and pass to the player's input
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  player.handleInput(keystate);

  //adding a gameover screen
  if (player.health <= 0) {
  UIManager::MenuOption gameOverSelected = UIManager::MenuOption::Start;
  bool gameOver = true;
  while (gameOver) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) return 0;
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
          gameOverSelected = (gameOverSelected == UIManager::MenuOption::Start) ? UIManager::MenuOption::Quit : UIManager::MenuOption::Start;
        if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
          gameOverSelected = (gameOverSelected == UIManager::MenuOption::Quit) ? UIManager::MenuOption::Start : UIManager::MenuOption::Quit;
        if (event.key.keysym.sym == SDLK_RETURN) {
          if (gameOverSelected == UIManager::MenuOption::Start)
            gameOver = false;
          else
            return 0;
        }
      }
    }
    ui.drawGameOverScreen(gameOverSelected);
    SDL_Delay(16);
  }
  break;
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
  CollisionManager::handleCollisions(player, enemies, bulletManager.bullets, enemyBullets,  powerUps, audio, bulletManager, score, lastCollectedPowerUp, hasActivePowerup);
  if(!powerUps.empty()) lastCollectedPowerUp = powerUps.back()->getType();


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

  //this will draw the entire hud
  ui.drawHUD(bulletManager.bulletsInMag, bulletManager.reloading, player.health, score, lastCollectedPowerUp, levelGen.getLevel(), hasActivePowerup);



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
}