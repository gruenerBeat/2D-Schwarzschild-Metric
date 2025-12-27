#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "GeometryHelper.h"
#include "Metric.h"
#include "RenderingUtils.h"

#define _USE_MATH_DEFINES
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

constexpr double timeSpan = 10;
constexpr double timeStep = 0.1;

constexpr vec3 camPolarPos{2, M_PI_2, 0};
constexpr vec3 camPolarDirection{-1, 0, 0};

constexpr int sphereCount = 1;
Sphere blackHole = Sphere{vec3{0, 0, 0}, 1, Color{255, 0, 255, 255}};
Sphere spheres[1] = {blackHole};

int main() {

  //Setup SDL
  std::cout << "Setup SDL";
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

  //Setup initial conditions
  std::cout << "Setup Initial";
  static Ray rays[SCREEN_WIDTH][SCREEN_HEIGHT];
  for(int x = 0; x < SCREEN_WIDTH; x++) {
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      rays[x][y].didHit = false;
      rays[x][y].hitColor = Color{0, 0, 0, 0};
      vec3 normVel = Normalize(camPolarDirection) * c;
      vec8 startState{
        camPolarPos.x, normVel.x,
        0, 0,
        camPolarPos.y, normVel.y,
        camPolarPos.z, normVel.z
      };
      rays[x][y].state = startState;
    }
  }

  bool notInterrupt = true;

  //Simulate
  std::cout << "Simulate";
  for(double t = 0; t <= timeSpan; t += timeStep) {

    //Interrupt
    while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) {
        notInterrupt = false;
		    t = timeSpan + 1;
	    }
	  }

    //For every Ray
    for(int x = 0; x < SCREEN_WIDTH; x++) {
      for(int y = 0; y < SCREEN_HEIGHT; y++) {
        if(!rays[x][y].didHit) {

          //Hit Check
          bool hit = false;
          for(int i = 0; i < sphereCount; i++) {
            //vec3 sphereRelative = PolarTransformation(CartesianTransformaion(vec3{rays[x][y].state.y1, rays[x][y].state.y5, rays[x][y].state.y7}) + (spheres[i].cartPos * -1));
            if(rays[x][y].state.y1 <= spheres[i].radius) {
              hit = true;
              rays[x][y].hitColor = spheres[i].color;
            }
          }
          if(hit) {
            rays[x][y].didHit = true;
            continue;
          }
          //Sim Step (RK4)
          vec8 oldState = rays[x][y].state;
          vec8 k1 = getYPrime(oldState);
          vec8 k2 = getYPrime(oldState + (k1 * (timeStep / 2)));
          vec8 k3 = getYPrime(oldState + (k2 * (timeStep / 2)));
          vec8 k4 = getYPrime(oldState + (k3 * timeStep));
          vec8 newState = oldState + ((k1 + (k2 * 2) + (k3 * 2) + k4) * timeStep * (1 / 6));
          rays[x][y].state = newState;

          //Normalisation
          double magnitude = std::sqrt(newState.y2 * newState.y2 + newState.y4 * newState.y4 + newState.y6 * newState.y6 + newState.y8 * newState.y8);
          rays[x][y].state.y2 = (newState.y2 / magnitude) * c;
          rays[x][y].state.y4 = (newState.y4 / magnitude) * c;
          rays[x][y].state.y6 = (newState.y6 / magnitude) * c;
          rays[x][y].state.y8 = (newState.y8 / magnitude) * c;

          //No Hit
          Color color = Color{0, 0, 0, 0};
          SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
          SDL_RenderDrawPoint(renderer, x, y);

        } else {

          //Draw
          Color color = rays[x][y].hitColor;
          SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
          SDL_RenderDrawPoint(renderer, x, y);
        }
      }
    }

    std::cout << t * (1 / timeStep) << " / " << timeSpan * (1 / timeStep) << "\n";

    //Render
    SDL_RenderPresent(renderer);
    SDL_Delay(1000 * timeStep);
  } 

  std::cout << "\nDONE!\n\n";

  while(notInterrupt) {
    while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) {
        notInterrupt = false;
	    }
	  }
  }

  //Do SDL stuff
  SDL_RenderPresent(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;
}

