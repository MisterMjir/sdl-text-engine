#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "text_engine.h"

// Prototypes
void input();
void update();
void draw(SDL_Renderer *);

// Constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

// Other globals
bool running = true;


// Text Engine
TextEngine* txtEng;

int main(int argc, char *args[])
{
  // Create window object
  SDL_Window *window = NULL;

  // Initialize video and create the window
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Text Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  // Create render object
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Initialize Text Engine
  TTF_Init();

  txtEng = new TextEngine("What's popping?", 64);

  while (running)
  {
    input();
    update();
    draw(renderer);
  }

  // Free up the memory
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();

  // Successful program termination
  return 0;
}

void input()
{
  SDL_Event e;
  if (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
      running = false;
  }
}

void update()
{
  txtEng->update();
}

void draw(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_Rect rect = {32, 32, 64, 1};
  SDL_RenderDrawRect(renderer, &rect);

  txtEng->draw(renderer);

  SDL_RenderPresent(renderer);
}
