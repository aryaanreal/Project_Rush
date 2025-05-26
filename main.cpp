//making core sdl functionality
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Player.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "Enemy.h"
#include "AudioManager.h"
#include "LevelGenerator.h"
#include <vector>
#include <memory>

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



//setting up main game loop now
while (isRunning) {
  //poll for any events like clsoing the window or key presses
 
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
          isRunning = false;
      }
    
    if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE) {
    bulletManager.tryFire(player.x, player.y, bulletTex);
    audio.playFire();     //play fire sfx
    }
  }

  }

  //getting keystate and pass to the player's input
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  player.handleInput(keystate);

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


  //drawing bg image
  SDL_RenderCopy(renderer, backgroundTex, nullptr, nullptr);

  //drawing the player texture 
  player.draw(renderer);

  //drawing bullets
  bulletManager.draw(renderer);

  //drawing enemies
  for(auto& enemy : enemies) {
    if(enemy.active) enemy.draw(renderer);
  }

  //draw enemy bullets 
  for (auto& bullet : enemyBullets) {
  if (bullet->active) bullet->draw(renderer);
}

  //writting on the bottom left

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