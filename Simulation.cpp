#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>

#define _USE_MATH_DEFINES
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCALE 50
#define THICKNESS 0.98

#include "GeometryHelper.h"
#include "ScreenHelper.h"
#include "Metric.h"

//Almost Stable Circular Orbit
//double pointR = 3;
//double pointT = 1;
//double velR = 1.2490181822;
//double velT = 1;

double pointR = 9;
double pointT = 9 * (M_PI_4 / 2);
double velR = -1;
double velT = 0.05;

vec8 y{pointR, velR, 0, 0.2, M_PI_2, 0, pointT, velT};
//vec8 y{0, 0, pointR, velR, pointT, velT, 0, 0};

constexpr double timeSpan = 10;
constexpr double timeStep = 0.01;

constexpr int movementRate = 2;

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
  SDL_Event event;
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
  bool space = false;
  bool paused = false;
  bool sPressed = false;
  vec2 windowPos{0, 0};

  //Normalisation
  double velocityMagnitude = std::sqrt(y.y2 * y.y2 + y.y4 * y.y4 + y.y6 * y.y6 + y.y8 * y.y8) != 0 ? std::sqrt(y.y2 * y.y2 + y.y4 * y.y4 + y.y6 * y.y6 + y.y8 * y.y8) : 1;
  y.y2 = (y.y2 / velocityMagnitude) * c;
  y.y4 = (y.y4 / velocityMagnitude) * c; 
  y.y6 = (y.y6 / velocityMagnitude) * c; 
  y.y8 = (y.y8 / velocityMagnitude) * c; 

  std::list<vec2> trajectory;
  for(int t = 0; t <= timeSpan; t += timeStep) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    //Event Handler
    do {
      while (SDL_PollEvent(&event)) {
        switch(event.type) {
          case SDL_QUIT:
            t = timeSpan + 1;
            break;
          case SDL_KEYDOWN:
            switch(event.key.keysym.scancode) {
              case SDL_SCANCODE_UP:
                up = true;
                break;
              case SDL_SCANCODE_DOWN:
                down = true;
                break;
              case SDL_SCANCODE_LEFT:
                left = true;
                break;
              case SDL_SCANCODE_RIGHT:
                right = true;
                break;
              case SDL_SCANCODE_SPACE:
                space = true;
                break;
            }
            break;
          case SDL_KEYUP:
            switch(event.key.keysym.scancode) {
              case SDL_SCANCODE_UP:
                up = false;
                break;
              case SDL_SCANCODE_DOWN:
                down = false;
                break;
              case SDL_SCANCODE_LEFT:
                left = false;
                break;
              case SDL_SCANCODE_RIGHT:
                right = false;
                break;
              case SDL_SCANCODE_SPACE:
                space = false;
                sPressed  = false;
                break;
            }
            break;
          default:
            break;
        }
        if(space && !sPressed) {
          sPressed = true;
          paused = !paused;
        }
        if(up) {
          windowPos.y += movementRate;
        }
        if(down) {
          windowPos.y -= movementRate;
        }
        if(left) {
          windowPos.x += movementRate;
        }
        if(right) {
          windowPos.x -= movementRate;
        }
	    }


    } while(paused);

    //Draw Coordinate Grid
    for(int x = 0; x < SCREEN_WIDTH; x++) {
      for(int y = 0; y < SCREEN_HEIGHT; y++) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //WHITE
        vec2 pos = TransformToSimulationCoords(x, y, windowPos);
        vec2 polarPosition = PolarTransformation(pos.x, pos.y);
        double r = polarPosition.x;
        double theta = polarPosition.y;
        if(std::fmod(r, 1) >= THICKNESS) {
          SDL_RenderDrawPoint(renderer, x, y);
        }
        if(r >= rs && r <= rs + THICKNESS / SCALE) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); //PURPLE
          SDL_RenderDrawPoint(renderer, x, y);
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //WHITE
    vec2 originOnScreen = TransformToScreenCoords(0, 0, windowPos);
    for(int i = 0; i < 8; i++) {
      int size = 500000;
      vec2 screenEndPoint = TransformToScreenCoords(size * std::cos(i * M_PI_4), size * std::sin(i * M_PI_4), windowPos);
      SDL_RenderDrawLine(renderer, originOnScreen.x, originOnScreen.y, screenEndPoint.x, screenEndPoint.y);
    }

    //Display Basis
    Basis basis = getPolarBasis(pointR, pointT);
    vec2 probepoint{pointR * -std::cos(pointT), pointR * std::sin(pointT)};
    vec2 rEndpoint{basis.e1.x + probepoint.x, basis.e1.y + probepoint.y};
    vec2 tEndpoint{basis.e2.x + probepoint.x, basis.e2.y + probepoint.y};
    vec2 probeOnScreen = TransformToScreenCoords(probepoint.x, probepoint.y, windowPos);
    vec2 rPointOnScreen = TransformToScreenCoords(rEndpoint.x, rEndpoint.y, windowPos);
    vec2 tPointOnScreen = TransformToScreenCoords(tEndpoint.x, tEndpoint.y, windowPos);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //RED
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, rPointOnScreen.x, rPointOnScreen.y);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //GREEN
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, tPointOnScreen.x, tPointOnScreen.y);

    //Explicit Euler
    y = y + (getYPrime(y) * timeStep);

    //Polar coords  (r dr t dt t dt p dp)
    pointR = y.y1;
    pointT = y.y7;

    //Cartesian coords  (t dt x dx y dy z dz)
    //pointR = std::sqrt(y.y3 * y.y3 + y.y5 * y.y5);
    //pointT = getAngle(y.y3, y.y5);

    //Add point to Trajectory
    vec2 cartesianPosition = CartesianTransformaion(pointR, pointT);
    trajectory.push_back(cartesianPosition);

    //Draw Trajectory
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //BLUE
    for(vec2 point : trajectory) {
      vec2 screenPoint = TransformToScreenCoords(point.x, point.y, windowPos);
      SDL_RenderDrawPoint(renderer, screenPoint.x, screenPoint.y);
    }

    //SDL stuff
    SDL_RenderPresent(renderer);
    SDL_Delay(1000 * timeStep);
  }


  //Do SDL stuff
  SDL_RenderPresent(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;
}

