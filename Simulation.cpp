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
#include "Metric.h"

double pointR;
double pointT;
TransformingVector velocity;

constexpr double startTheta = M_PI_4;
constexpr double startR = 1;
constexpr double timeSpan = 10;
constexpr double timeStep = 0.01;
constexpr vec2 initialVelocity{0.2, 1};

//Schwarzschild
constexpr double M = 1;
constexpr double N = 100;
constexpr double h = 0.1;


struct vec8
{
    double y1;
    double y2;
    double y3;
    double y4;
    double y5;
    double y6;
    double y7;
    double y8;
};


vec8 yStart{2, 0, 0, 0, 0, 0, 0, 1};


/*double
cot(double x)
{
    return std::tan(x) != 0 ? 1 / std::tan(x) : 0;
}*/


vec8
vectorAddition(vec8 a, vec8 b)
{
    return vec8{a.y1 + b.y1, a.y2 + b.y2, a.y3 + b.y3, a.y4 + b.y4, a.y5 + b.y5, a.y6 + b.y6, a.y7 + b.y7, a.y8 + b.y8};
}


vec8
scal(double a, vec8 b)
{
    return vec8{a * b.y1, a * b.y2, a * b.y3, a * b.y4, a * b.y5, a * b.y6, a * b.y7, a * b.y8};
}


vec8
getYPrime(vec8 y)
{
    double a = y.y1 - 2 * M;
    vec8 yPrime{y.y2, -(M * a / (y.y1 * y.y1 * y.y1)) * y.y4 * y.y4 + M / (y.y1 * a) * y.y2 * y.y2 + a * y.y6 * y.y6 + a * std::sin(y.y5) * std::sin(y.y5) * y.y8 * y.y8,
                y.y4, -2 * M / (a * y.y1) * y.y2 * y.y4,
                y.y6, -2 / y.y1 * y.y2 * y.y6 + std::sin(y.y5) * std::cos(y.y5) * y.y8 * y.y8,
                y.y8, -2 / y.y1 * y.y2 * y.y8 - 2 * cot(y.y5) * y.y6 * y.y8};
    return yPrime;
}

//


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
  vec8 y = yStart;
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

    //Explizites Euler
    y = vectorAddition(y, scal(timeStep, y));

    //Draw Position
    pointR = y.y1;
    pointT = y.y7;

    //Update coords
    /*TransformingVector cartesianVelocity = PolarToOrthonormalBasis(velocity);
    vec2 cartesianPosition = CartesianTransformaion(pointR, pointT);
    cartesianPosition.x += cartesianVelocity.components.x * timeStep;
    cartesianPosition.y += cartesianVelocity.components.y * timeStep;
    vec2 newPolar = PolarTransformation(cartesianPosition.x, cartesianPosition.y);
    pointR = newPolar.x;
    pointT = newPolar.y;

    //Update velocity
    velocity = UpdateBasis(velocity, pointR, pointT);
    velocity = GetAcceleration(velocity, pointR, pointT);*/

    //Draw velocity
    /*vec2 rawCartesianVelocity{cartesianVelocity.components.x, cartesianVelocity.components.y};
    vec2 absoluteVelocity = TransformToScreenCoords(rawCartesianVelocity.x + probepoint.x, rawCartesianVelocity.y + probepoint.y);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); //YELLOW
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, absoluteVelocity.x, absoluteVelocity.y);
    //???
    vec2 velCompR{velocity.basis.e1.x * velocity.components.x, velocity.basis.e1.y * velocity.components.x};
    vec2 velCompT{velocity.basis.e2.x * velocity.components.y, velocity.basis.e2.y * velocity.components.y};
    vec2 cartCompR = CartesianTransformaion(velCompR.x, velCompR.y);
    vec2 cartCompT = CartesianTransformaion(velCompT.x, velCompT.y);
    vec2 onScreenCompR = TransformToScreenCoords(cartCompR.x + probepoint.x, cartCompR.y + probepoint.y);
    vec2 onScreenCompT = TransformToScreenCoords(cartCompT.x + probepoint.x, cartCompT.y + probepoint.y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //BLUE
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, onScreenCompR.x, onScreenCompR.y);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); //CYAN
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, onScreenCompT.x, onScreenCompT.y);*/

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

