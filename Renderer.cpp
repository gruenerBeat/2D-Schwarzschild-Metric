#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>

#include "GeometryHelper.h"
#include "Screen.h"

#define _USE_MATH_DEFINES
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCALE 50

#define LIMIT 100000
#define STEP 0.01
constexpr double GRIDSIZE = 1;

struct Probe{
  vec2 position;
  double distance;
  int movingBasis;
  Basis basis;
  bool first;
};

int main() {

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
  SDL_Event event;

  bool running = true;
  int index = 0;
  std::vector<Probe> probes;
  std::list<vec2> greenPoints;
  std::list<vec2> redPoints;
  int pSize = 2;
  probes.push_back(Probe{vec2{0, 0}, 0, 0, true});
  probes.push_back(Probe{vec2{0, 0}, 0, 1, true});

  while(running && index <= LIMIT) {

    for(int i = 0; i < pSize; i++) {


      Probe p = probes[i];
      if(fmod(p.distance, GRIDSIZE) == 0 && p.first) {
        probes.push_back(Probe{p.position, 0, p.movingBasis == 0 ? 1 : 0, false});
      }

      p.basis = getBasis(p.position);

      if(p.movingBasis == 0) {
        redPoints.push_back(p.position);
        p.position = p.position + (p.basis.e1 * STEP);
      } else {
        greenPoints.push_back(p.position);
        p.position = p.position + (p.basis.e2 * STEP);
      }

      p.distance += STEP;
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for(vec2 v : redPoints) {
      vec2 screen = ToScreenCoords(v, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE);
      SDL_RenderDrawPoint(renderer, screen.x, screen.y);
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for(vec2 v : redPoints) {
      vec2 screen = ToScreenCoords(v, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE);
      SDL_RenderDrawPoint(renderer, screen.x, screen.y);
    }

    pSize == sizeof(probes) / sizeof(Probe);
    index++;
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;
}

