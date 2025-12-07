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
constexpr double startR = 2;
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
        double r = std::sqrt(pos.x * pos.x + pos.y * pos.y);
        double theta = getAngle(pos.x, pos.y);
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
    vec2 coords = CartesianTransformaion(pointR, pointT);
    coords.x += velocity.components.x * timeStep;
    coords.y += velocity.components.y * timeStep;
    pointR = std::sqrt(coords.x * coords.x + coords.y * coords.y);
    pointT = getAngle(coords.x, coords.y);

    //Update velocity
    /*Basis newBasis = getPolarBasis(pointR, pointT);
    vec2 basisRadiusChange{(newBasis.e1.x - basis.e1.x) / timeStep, (newBasis.e1.y - basis.e1.y) / timeStep};
    vec2 basisThetaChange{(newBasis.e2.x - basis.e2.x) / timeStep, (newBasis.e2.y - basis.e2.y) / timeStep};
    double rChangeMagnitude = std::sqrt(basisRadiusChange.x * basisRadiusChange.x + basisRadiusChange.y * basisRadiusChange.y);
    double tChangeMagnitude = std::sqrt(basisThetaChange.x * basisThetaChange.x + basisThetaChange.y * basisThetaChange.y);
    velocity.basis = newBasis;
    velocity.components.x *= rChangeMagnitude;
    velocity.components.y *= tChangeMagnitude;*/

    //Display velocity
    /*vec2 cartesianvelocity = ToOrthonormalBasis(velocity);
    vec2 cartesianvelocityOnScreen = TransformToScreenCoords(cartesianvelocity.x, cartesianvelocity.y);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); //AQUAMRINE
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, cartesianvelocityOnScreen.x, cartesianvelocityOnScreen.y);
    vec2 scaledRadius{basis.e1.x * velocity.components.x, basis.e1.y * velocity.components.x};
    vec2 scaledTheta{basis.e2.x * velocity.components.y, basis.e2.y * velocity.components.y};
    vec2 scaledRadiusOnScreen = TransformToScreenCoords(scaledRadius.x, scaledRadius.y);
    vec2 scaledThetaOnScreen = TransformToScreenCoords(scaledTheta.x, scaledTheta.y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //BLUE
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, scaledRadiusOnScreen.x, scaledRadiusOnScreen.y);
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, scaledThetaOnScreen.x, scaledThetaOnScreen.y);*/

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

