#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define SCALE 5

constexpr double startTheta = M_PI_4;
constexpr double startR = 50;


struct vec2 {
  double x;
  double y;  
};

struct Basis {
  double rX;
  double rY;
  double tX;
  double tY;
};

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

Basis getPolarBasis(double r, double theta) {
  Basis basis;
  basis.rX = std::cos(theta);
  basis.rY = std::sin(theta);
  basis.tX = r * r * std::cos(M_PI - (theta + M_PI_2));
  basis.tY = r * r * std::sin(M_PI - (theta + M_PI_2));
  return basis;
}

vec2 TransformScreenCoords(double x, double y) {
  vec2 vector;
  vector.x = ((SCREEN_WIDTH / 2) - x) / SCALE;
  vector.y = ((SCREEN_HEIGHT / 2) - y) / SCALE;
  return vector;
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
      vec2 pos = TransformScreenCoords(x, y);
      double r = std::sqrt(pos.x * pos.x + pos.y * pos.y);
      double theta = getAngle(pos.x, pos.y);
      if(std::fmod(r, 1) <= 0.9 || std::fmod(theta, M_PI_4) >= M_PI_4 - (2 / r)) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }

  //Draw Basis
  Basis basis = getPolarBasis(startR, startTheta);
  vec2 probepoint{-std::ceil(startR * std::cos(startTheta)), std::ceil(startR * std::sin(startTheta))};
  vec2 rEndpoint{probepoint.x + basis.rX, probepoint.y + basis.rY};
  vec2 tEndpoint{probepoint.x + basis.tX, probepoint.y + basis.tY};
  vec2 originOnScreen = TransformScreenCoords(probepoint.x, probepoint.y);
  vec2 rPointOnScreen = TransformScreenCoords(rEndpoint.x, rEndpoint.y);
  vec2 tPointOnScreen = TransformScreenCoords(tEndpoint.x, tEndpoint.y);
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
  SDL_RenderDrawLine(renderer, originOnScreen.x, originOnScreen.y, rPointOnScreen.x, rPointOnScreen.y);
  SDL_RenderDrawLine(renderer, originOnScreen.x, originOnScreen.y, tPointOnScreen.x, tPointOnScreen.y);

  //Do SDL stuff
  SDL_RenderPresent(renderer);
  SDL_Delay(10000);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;
}

