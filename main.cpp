//making core sdl functionality
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <iostream>
#include "Player.h"
#include "Bullet.h"
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

//creating the bullet surface
SDL_Texture* bulletTex = SDL_CreateTextureFromSurface(renderer, bulletSurface);
SDL_FreeSurface(bulletSurface);


//game loop control variable
bool isRunning = true;

//SDL event variables to track key presses, quitting and stuff
SDL_Event event;

//PLayer's position on the screen

Player player(400, 500, playerTex);     //using the player start position func inherited from entity

Bullet bullets[100];  //initial 100 bullets
int bulletcount = 0;


//setting up main game loop now
while (isRunning) {
  //poll for any events like clsoing the window or key presses
 
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
          isRunning = false;
      }

      if (event.type == SDL_KEYDOWN) {
        std::cout << "Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
        //bullet fire logic
        if (event.key.keysym.sym == SDLK_SPACE) {
          if (bulletcount < 100){
            bullets[bulletcount++] = Bullet(player.x + 24.0f, player.y, bulletTex);
            std::cout << "Bullet fired! Count: " << bulletcount << std::endl;
          }
        }
      }
    }

  //getting keystate and pass to the player's input
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  player.handleInput(keystate);

  //update bulletss
  for (int i = 0; i < bulletcount; i++){
    if(bullets[i].active) {
      bullets[i].move();
    }
  }

  //set the bg color to black
  SDL_SetRenderDrawColor(renderer, 0 , 0 , 0 , 255);
  SDL_RenderClear(renderer); //clear the screen with said color

  //drawing the player texture 
  player.draw(renderer);

  //drawing bullets
  for (int i = 0; i < bulletcount; i ++){
    if (bullets[i].active) {
      bullets[i].draw(renderer);
    }
  }

  //showing everything we drew
  SDL_RenderPresent(renderer);

  //delay to make framerate (60 fps)
  SDL_Delay(16);


}

//clean up all sdl resources to save computing power
SDL_DestroyTexture(bulletTex);
SDL_DestroyTexture(playerTex);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
IMG_Quit();
SDL_Quit();

return 0; //exit program

}