#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define SCALE 5


struct Basis {
  double rX;
  double rY;
  double tX;
  double tY;
}

double getAngle(double x, double y) {
  if(x == 0 && -y > 0) {
    return M_PI_2;
  } else if(x == 0 && -y >= 0) {
    return 3 * M_PI_2;
  } else if(x < 0 && -y <= 0) {
    return std::atan(-y / x);
  } else if(x < 0 && -y > 0) {
    return std::atan(-y / x) + 2 * M_PI;
  } else if (x > 0) {
    return std::atan(-y / x) + M_PI;
  } else {
    return 0;
  }
}

double getPolarBasis(double r, double theta) {
  Basis basis;
  basis.rX = std::cos(theta);
  basis.rY = std::sin(theta);
  basis.tX = r * r * std::cos(theta + M_PI_2);
  basis.tY = r * r * std::sin(theta + M_PI_2);
  return basis;
}

int main(){

  //Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Window *window = SDL_CreateWindow("Example: 0", SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!window){
    printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  //Draw Coordinate Grid
  for(int x = 0; x < SCREEN_WIDTH; x++) {
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      int xPos = (SCREEN_WIDTH / 2) - x;
      int yPos = (SCREEN_HEIGHT / 2) - y;
      double r = std::sqrt(xPos * xPos + yPos * yPos);
      double theta = getAngle(xPos, yPos);
      if(std::fmod(r, (10 * SCALE)) >= (10 * SCALE) - 1 || std::fmod(theta, M_PI_4) >= M_PI_4 - (2 / r)) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }

  //Do SDL stuff
  SDL_RenderPresent(renderer);
  SDL_Delay(10000);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;
}

