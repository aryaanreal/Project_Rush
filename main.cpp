//making core sdl functionality
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <iostream>

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


//game loop control variable
bool isRunning = true;

//SDL event variables to track key presses, quitting and stuff
SDL_Event event;

//PLayer's position on the screen
float playerX = 400, playerY = 500;


//setting up main game loop now
while (isRunning) {
  //poll for any events like clsoing the window or key presses
  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
        isRunning = false; //exit game
    }
  }

  //set the bg color to black
  SDL_SetRenderDrawColor(renderer, 0 , 0 , 0 , 255);
  SDL_RenderClear(renderer); //clear the screen with said color

  //drawing the player texture 
  SDL_Rect dst = {static_cast<int>(playerX), static_cast<int>(playerY),64,64};
  SDL_RenderCopy(renderer, playerTex,nullptr, &dst);

  //showing everything we drew
  SDL_RenderPresent(renderer);

  //delay to make framerate (60 fps)
  SDL_Delay(16);
}

//clean up all sdl resources to save computing power
SDL_DestroyTexture(playerTex);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
IMG_Quit();
SDL_Quit();

return 0; //exit program

}