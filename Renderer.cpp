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

#define BOUNDARY 20
constexpr Color background{10, 10, 10, 10};
constexpr Color black_hole{0, 0, 0, 0};

constexpr double timeSpan = 1000;
constexpr double timeStep = 0.1;

vec3 camCartesianWorldPos{3, 0, -2};
vec3 camCartesianWorldDirection{-1, 0, 0};
constexpr double fov = 60;
constexpr double pixelSize = 0.05;

Sphere spheres[] = {
  Sphere{vec3{-2, -3, 2}, 1, Color{255, 127, 0, 255}},
  Sphere{vec3{-1, 4, 0}, 1, Color{255, 0, 255, 255}},
  Sphere{vec3{-4, -1, -1}, 1, Color{255, 0, 0, 255}},
};
int sphereCount = sizeof(spheres) / sizeof(spheres[0]);

int main() {
  double f = ((SCREEN_HEIGHT * pixelSize) / 2) * cot((M_PI * fov / 180) / 2);
  std::cout << f << std::endl;
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

  //Setup initial conditions
  static Ray rays[SCREEN_WIDTH][SCREEN_HEIGHT];
  for(int x = 0; x < SCREEN_WIDTH; x++) {
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      rays[x][y].didHit = false;
      rays[x][y].hitColor = Color{20, 20, 20, 255};
      rays[x][y].hitDst = 1000;

      //Calculate Ray Direction from Pixel
      Matrix3x3 IntrinsicMatrix = Matrix3x3{vec3{f, 0, 0}, vec3{0, f, 0}, vec3{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1}};
      vec3 pointInCamSpace = !IntrinsicMatrix * vec3{x, y, 1};
      vec3 rightVector = Normalize(Cross(vec3{0, 1, 0}, camCartesianWorldDirection));
      Matrix4x4 RotationMatrix = Matrix4x4{
        ToVec4(rightVector, camCartesianWorldPos.x),
        ToVec4(Normalize(Cross(rightVector, camCartesianWorldDirection)), camCartesianWorldPos.y),
        ToVec4(camCartesianWorldDirection, camCartesianWorldPos.z),
        vec4{0, 0, 0, 1}
      };
      vec3 pointInWorldSpace = ToVec3(Inverse(RotationMatrix) * ToVec4(pointInCamSpace - camCartesianWorldPos, 0));
      vec3 rayDirection = Normalize(camCartesianWorldPos - pointInWorldSpace);

      //Transform
      vec3 camPolarPos = PolarTransformation(camCartesianWorldPos);
      vec3 polarVelocity = SphericalTransformationAt(camPolarPos, rayDirection) * c;

      //Apply
      vec8 startingState = vec8{
        camPolarPos.x, polarVelocity.x,
        0, 0,
        camPolarPos.y, polarVelocity.y,
        camPolarPos.z, polarVelocity.z
      };
      rays[x][y].state = startingState;

      SDL_SetRenderDrawColor(renderer, rays[x][y].hitColor.r, rays[x][y].hitColor.g, rays[x][y].hitColor.b, rays[x][y].hitColor.a);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }

  SDL_RenderPresent(renderer);
  bool notInterrupt = true;

  //Simulate
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
            vec3 cartPos = CartesianTransformaion(vec3{rays[x][y].state.y1, rays[x][y].state.y5, rays[x][y].state.y7});
            vec3 relativeToSphere = cartPos - spheres[i].cartPos;
            double r = std::sqrt(relativeToSphere.x * relativeToSphere.x + relativeToSphere.y * relativeToSphere.y + relativeToSphere.z * relativeToSphere.z);
            if(r <= spheres[i].radius) {
              hit = true;
              rays[x][y].hitColor = spheres[i].color;
            }
          }
          if(rays[x][y].state.y1 >= BOUNDARY) {
            hit = true;
            rays[x][y].hitColor = background;
          }
          if(rays[x][y].state.y1 <= rs) {
            hit = true;
            rays[x][y].hitColor = black_hole;
          }
          if(hit) {
            rays[x][y].didHit = true;
            continue;
          }

          //Sim Step (RK4)
          vec8 k1 = getYPrime(rays[x][y].state);
          vec8 k2 = getYPrime(rays[x][y].state + k1 * (timeStep / 2));
          vec8 k3 = getYPrime(rays[x][y].state + k2 * (timeStep / 2));
          vec8 k4 = getYPrime(rays[x][y].state + k3 * timeStep);
          rays[x][y].state = rays[x][y].state + (k1 + (k2 * 2) + (k3 * 2) + k4) * (timeStep / 6);

          //Normalisation
          double magnitude = std::sqrt(rays[x][y].state.y2 * rays[x][y].state.y2 + rays[x][y].state.y4 * rays[x][y].state.y4 + rays[x][y].state.y6 * rays[x][y].state.y6 + rays[x][y].state.y8 * rays[x][y].state.y8);
          rays[x][y].state.y2 = (rays[x][y].state.y2 / magnitude) * c;
          rays[x][y].state.y4 = (rays[x][y].state.y4 / magnitude) * c;
          rays[x][y].state.y6 = (rays[x][y].state.y6 / magnitude) * c;
          rays[x][y].state.y8 = (rays[x][y].state.y8 / magnitude) * c;

          //No Hit
          Color color = Color{20, 20, 20, 255};
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

    //Render
    SDL_RenderPresent(renderer);
  }

  std::cout << "BReak\n";

  bool done = true;
  while(done) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        notInterrupt = false;
        done = false;
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

