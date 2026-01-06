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

//CONVENTION:
//
//world:
//y -> up
//
//cam:
//y -> up
//z -> viewing direction
//x -> right

vec3 camCartesianWorldPos{5, 5, -5};
vec3 camCartesianWorldDirection{1, 1, -1};
constexpr double hfov = M_PI_2;

constexpr int sphereCount = 1;
Sphere blackHole = Sphere{vec3{0, 0, 0}, 1, Color{255, 0, 255, 255}};
Sphere spheres[sphereCount] = {blackHole};

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

  //Setup initial conditions
  int index = 0;
  static Ray rays[SCREEN_WIDTH][SCREEN_HEIGHT];
  for(int x = 0; x < SCREEN_WIDTH; x++) {
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      rays[x][y].didHit = false;
      rays[x][y].hitColor = Color{0, 0, 0, 0};
      rays[x][y].hitDst = 100000000;

      //Calculate Ray Direction from Pixel
      vec3 pixelInNDC = ToNDC(x, y, SCREEN_WIDTH, SCREEN_HEIGHT);
      double f = 1 / (std::tan(hfov/ 2));
      vec3 pointInCamSpace = vec3{pixelInNDC.x, pixelInNDC.z, f};
      vec3 pointInSemiWorldSpace = pointInCamSpace + camCartesianWorldPos;
      vec3 camBasisZInWorld = Normalize(camCartesianWorldDirection * -1);
      vec3 camBasisXInWorld = Normalize(Cross(camBasisZInWorld, vec3{0, 1, 0}));
      vec3 camBasisYInWorld = Normalize(Cross(camBasisXInWorld, camBasisZInWorld));
      Matrix3x3 camToWorldMatrix = Matrix3x3{camBasisXInWorld, camBasisYInWorld, camBasisZInWorld};
      vec3 pointInWorldSpace = camToWorldMatrix * pointInSemiWorldSpace;
      vec3 rayDirection = Normalize(pointInCamSpace);

      std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << "\n";
 
      for(int i = 0; i < sphereCount; i++) {
        double dst;

        vec3 relativeRayPos = camCartesianWorldPos * -1 - spheres[i].cartPos;
        double a = Dot(rayDirection, rayDirection);
        double b = 2 * Dot(rayDirection, relativeRayPos);
        double c = Dot(relativeRayPos, relativeRayPos) - spheres[i].radius * spheres[i].radius;
        double discriminant = b * b - 4 * a * c;

        if(discriminant < 0) {
          continue;
        }

        double root = std::sqrt(discriminant);

        if(discriminant == 0) {
          dst = -b / (2 * a);
        } else {
          double dst1 = (-b + root) / (2 * a);
          double dst2 = (-b - root) / (2 * a);
          dst = dst1 >= dst2 ? dst2 : dst1;
        }

        if(dst <= rays[x][y].hitDst) {
          rays[x][y].didHit = true;
          rays[x][y].hitDst = dst;
          rays[x][y].hitColor = spheres[i].color;
        }
      }

      /*//Transform to Polar Coordinates
      vec3 polarRayPos = PolarTransformation(camCartesianWorldPos);
      vec3 polarRayDir = PolarTransformationAt(polarRayPos, rayDirection);

      //Apply
      vec8 startingState = vec8{
        polarRayPos.x, polarRayDir.x,
        0, 0,
        polarRayPos.y, polarRayDir.y,
        polarRayPos.z, polarRayDir.z
      };
      rays[x][y].state = startingState;*/

      SDL_SetRenderDrawColor(renderer, rays[x][y].hitColor.r, rays[x][y].hitColor.g, rays[x][y].hitColor.b, rays[x][y].hitColor.a);
      SDL_RenderDrawPoint(renderer, x, y);
      index++;
    }
  }

  std::cout << "Done";

  SDL_RenderPresent(renderer);

  bool notInterrupt = true;
  while(notInterrupt) {
    while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) {
        notInterrupt = false;
	    }
	  }
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;


  //Simulate
  /*for(double t = 0; t <= timeSpan; t += timeStep) {

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
            //double r = std::sqrt(rays[x][y].state.y1 * rays[x][y].state.y1 + rays[x][y].state.y5 * rays[x][y].state.y5 + rays[x][y].state.y7 * rays[x][y].state.y7);
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
          Color color = Color{0, 0, 0, 0};
          SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
          SDL_RenderDrawPoint(renderer, x, y);

          if(x == SCREEN_WIDTH / 2 && y == SCREEN_HEIGHT / 2) {
            std::cout << rays[x][y].state.y1 << " " << rays[x][y].state.y5 << " " << rays[x][y].state.y7 << "\n";
          }

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
  }*/

  

  //Do SDL stuff
  /*SDL_RenderPresent(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;*/
}

