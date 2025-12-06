#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define SCALE 50
#define THICKNESS 0.98


double pointR;
double pointT;

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

struct TransformingVector {
  Basis basis;
  vec2 components;
};

TransformingVector velocity;


constexpr double startTheta = M_PI_4;
constexpr double startR = 2;
constexpr double timeSpan = 10;
constexpr double timeStep = 0.01;
constexpr vec2 initialVelocity{1, 0};


double getAngle(double x, double y) {
  if(x == 0 && y == 0) {
    return 0;
  }
  return std::atan2(y, -x);
}

Basis getPolarBasis(double r, double theta) {
  Basis basis;
  basis.rX = -std::cos(theta);
  basis.rY = std::sin(theta);
  basis.tX = r * std::cos(M_PI - (theta + M_PI_2));
  basis.tY = r * std::sin(M_PI - (theta + M_PI_2));
  return basis;
}

//Transformations between screen and worldspace
vec2 TransformToScreenCoords(double x, double y) {
  vec2 vector;
  vector.x = (SCREEN_WIDTH / 2) - (x * SCALE);
  vector.y = (SCREEN_HEIGHT / 2) - (y * SCALE);
  return vector;
}
vec2 TransformToSimulationCoords(double x, double y) {
  vec2 vector;
  vector.x = ((SCREEN_WIDTH / 2) - x) / SCALE;
  vector.y = ((SCREEN_HEIGHT / 2) - y) / SCALE;
  return vector;
}

vec2* CalcThetaLines() {
  int maxR = std::max(SCREEN_HEIGHT, SCREEN_WIDTH);
  static vec2 endpoints[8];
  for(int i = 0; i < 8; i++) {
    endpoints[i].x = maxR * std::cos(i * M_PI_4);
    endpoints[i].y = maxR * std::sin(i * M_PI_4);
  }
  return endpoints;
}

vec2 CartesianTransformaion(float r, float t) {
  vec2 coords{r * std::cos(t), r * std::sin(t)};
  return coords;
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
    vec2 rEndpoint{basis.rX + probepoint.x, basis.rY + probepoint.y};
    vec2 tEndpoint{basis.tX + probepoint.x, basis.tY + probepoint.y};
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
    vec2 basisRadiusChange{(newBasis.rX - basis.rX) / timeStep, (newBasis.rY - basis.rY) / timeStep};
    vec2 basisThetaChange{(newBasis.tX - basis.tX) / timeStep, (newBasis.tY - basis.tY) / timeStep};
    double rChangeMagnitude = std::sqrt(basisRadiusChange.x * basisRadiusChange.x + basisRadiusChange.y * basisRadiusChange.y);
    double tChangeMagnitude = std::sqrt(basisThetaChange.x * basisThetaChange.x + basisThetaChange.y * basisThetaChange.y);
    velocity.basis = newBasis;
    velocity.components.x *= rChangeMagnitude;
    velocity.components.y *= tChangeMagnitude;*/

    //Display Velocity
    /*vec2 cartesianVelocity = ToOrthonormalBasis(velocity);
    vec2 cartesianVelocityOnScreen = TransformToScreenCoords(cartesianVelocity.x, cartesianVelocity.y);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); //AQUAMRINE
    SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y, cartesianVelocityOnScreen.x, cartesianVelocityOnScreen.y);
    vec2 scaledRadius{basis.rX * velocity.components.x, basis.rY * velocity.components.x};
    vec2 scaledTheta{basis.tX * velocity.components.y, basis.tY * velocity.components.y};
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

