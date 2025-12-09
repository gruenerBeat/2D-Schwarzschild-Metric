#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCALE 50
#define THICKNESS 0.98

#include "GeometryHelper.h"
#include "ScreenHelper.h"

double pointR;
double pointT;
TransformingVector velocity;

constexpr double startTheta = M_PI_4;
constexpr double startR = 1;
constexpr double timeSpan = 10;
constexpr double timeStep = 0.01;
constexpr vec2 initialVelocity{1, 0};

vec2* CalcThetaLines() {
  int maxR = std::max(SCREEN_HEIGHT, SCREEN_WIDTH);
  static vec2 endpoints[8];
  for(int i = 0; i < 8; i++) {
    endpoints[i].x = maxR * std::cos(i * M_PI_4);
    endpoints[i].y = maxR * std::sin(i * M_PI_4);
  }
  return endpoints;
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


  velocity.basis = getPolarBasis(startR, startTheta);
  velocity.components = initialVelocity;
  pointR = startR;
  pointT = startTheta;
  for(int t = 0; t <= timeSpan; t += timeStep) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    //Draw Coordinate Grid
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //WHITE
    for(int x = 0; x < SCREEN_WIDTH; x++) {
      for(int y = 0; y < SCREEN_HEIGHT; y++) {
        vec2 pos = TransformToSimulationCoords(x, y);
        vec2 polarPosition = PolarTransformation(pos.x, pos.y);
        double r = polarPosition.x;
        double theta = polarPosition.y;
        if(std::fmod(r, 1) >= THICKNESS) {
          SDL_RenderDrawPoint(renderer, x, y);
        }/* else {
          SDL_SetRenderDrawColor(renderer, (theta / (2 * M_PI)) * 255, 0, 0, 255);
          SDL_RenderDrawPoint(renderer, x, y);
        }*/
      }
    }
    vec2 originOnScreen = TransformToScreenCoords(0, 0);
    int maxR = std::max(SCREEN_HEIGHT, SCREEN_WIDTH);
    for(int i = 0; i < 8; i++) {
      int size = std::max(SCREEN_WIDTH, SCREEN_HEIGHT) / 50;
      vec2 screenEndPoint = TransformToScreenCoords(size * std::cos(i * M_PI_4), size * std::sin(i * M_PI_4));
      SDL_RenderDrawLine(renderer, originOnScreen.x, originOnScreen.y, screenEndPoint.x, screenEndPoint.y);
    }

    //Display Basis
    Basis basis = getPolarBasis(pointR, pointT);
    vec2 probepoint{pointR * -std::cos(pointT), pointR * std::sin(pointT)};
    vec2 rEndpoint{basis.e1.x + probepoint.x, basis.e1.y + probepoint.y};
    vec2 tEndpoint{basis.e2.x + probepoint.x, basis.e2.y + probepoint.y};
    vec2 probeOnScreen = TransformToScreenCoords(probepoint.x, probepoint.y);
    vec2 rPointOnScreen = TransformToScreenCoords(rEndpoint.x, rEndpoint.y);
    vec2 tPointOnScreen = TransformToScreenCoords(tEndpoint.x, tEndpoint.y);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //RED
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, rPointOnScreen.x, rPointOnScreen.y);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //GREEN
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, tPointOnScreen.x, tPointOnScreen.y);

    //Update coords
    TransformingVector cartesianVelocity = PolarToOrthonormalBasis(velocity);
    vec2 cartesianPosition = CartesianTransformaion(pointR, pointT);
    cartesianPosition.x += cartesianVelocity.components.x * timeStep;
    cartesianPosition.y += cartesianVelocity.components.y * timeStep;
    vec2 newPolar = PolarTransformation(cartesianPosition.x, cartesianPosition.y);
    pointR = newPolar.x;
    pointT = newPolar.y;

    //Update velocity
    velocity = UpdateBasis(velocity, pointR, pointT, timeSpan);
    

    //Draw velocity
    vec2 rawCartesianVelocity{cartesianVelocity.components.x, cartesianVelocity.components.y};
    vec2 absoluteVelocity = TransformToScreenCoords(rawCartesianVelocity.x + probepoint.x, rawCartesianVelocity.y + probepoint.y);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); //YELLOW
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, absoluteVelocity.x, absoluteVelocity.y);

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

