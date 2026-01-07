#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCALE 50
#define THICKNESS 0.98
#define boundary 15

bool text = false;
bool tp = true;

#include "GeometryHelper.h"
#include "Metric.h"
#include "ScreenHelper.h"

vec8 y[] = {
    {0.9 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {0.95 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {0.98 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {0.99 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.00 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.01 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.02 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.05 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.10 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.15 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.20 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.5 * rs - 1. / 8, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.5 * rs + 1. / 8, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.5 * rs - 1. / 16, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
    {1.5 * rs + 1. / 16, 0, 0, 0, M_PI_2, 0, M_PI_2, 1},
};
constexpr int pointSize = sizeof(y) / sizeof(y[0]);

constexpr double timeSpan = 100;
constexpr double timeStep = 0.01;

constexpr int movementRate = 2;

int main() {

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Window *window =
      SDL_CreateWindow("Example: 0", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!window) {
    printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT,
           SDL_GetError());
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

  for (int i = 0; i < pointSize; i++) {
    // Initial Normalisation
    double velocityMagnitude =
        std::sqrt(y[i].y2 * y[i].y2 + y[i].y4 * y[i].y4 + y[i].y6 * y[i].y6 +
                  y[i].y8 * y[i].y8) != 0
            ? std::sqrt(y[i].y2 * y[i].y2 + y[i].y4 * y[i].y4 +
                        y[i].y6 * y[i].y6 + y[i].y8 * y[i].y8)
            : 1;
    y[i].y2 = (y[i].y2 / velocityMagnitude) * c;
    y[i].y4 = (y[i].y4 / velocityMagnitude) * c;
    y[i].y6 = (y[i].y6 / velocityMagnitude) * c;
    y[i].y8 = (y[i].y8 / velocityMagnitude) * c;
  }

  std::list<vec2> trajectory[pointSize];
  for (int t = 0; t <= timeSpan; t += timeStep) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Event Handler
    do {
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
          t = timeSpan + 1;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
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
          switch (event.key.keysym.scancode) {
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
            sPressed = false;
            break;
          }
          break;
        default:
          break;
        }
        if (space && !sPressed) {
          sPressed = true;
          paused = !paused;
        }
        if (up) {
          windowPos.y += movementRate;
        }
        if (down) {
          windowPos.y -= movementRate;
        }
        if (left) {
          windowPos.x += movementRate;
        }
        if (right) {
          windowPos.x -= movementRate;
        }
      }
    } while (paused);

    // Draw Coordinate Grid
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      for (int y = 0; y < SCREEN_HEIGHT; y++) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // WHITE
        vec2 pos = TransformToSimulationCoords(x, y, windowPos);
        vec2 polarPosition = PolarTransformation(pos.x, pos.y);
        double r = polarPosition.x;
        double theta = polarPosition.y;
        if (std::fmod(r, 1) >= THICKNESS) {
          SDL_RenderDrawPoint(renderer, x, y);
        }
        if (r >= rs && r <= rs + THICKNESS / SCALE) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // PURPLE
          SDL_RenderDrawPoint(renderer, x, y);
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // WHITE
    vec2 originOnScreen = TransformToScreenCoords(0, 0, windowPos);
    for (int i = 0; i < 8; i++) {
      int size = 500000;
      vec2 screenEndPoint = TransformToScreenCoords(
          size * std::cos(i * M_PI_4), size * std::sin(i * M_PI_4), windowPos);
      SDL_RenderDrawLine(renderer, originOnScreen.x, originOnScreen.y,
                         screenEndPoint.x, screenEndPoint.y);
    }

    for (int i = 0; i < pointSize; i++) {

      // Velocity
      double vT = y[i].y4;
      double vR = y[i].y2;
      double vA = y[i].y8;
      double vP = y[i].y6;

      double pointR = y[i].y1;
      double pointT = y[i].y7;

      double velocity = std::sqrt(vT * vT + vR * vR + vA * vA + vP * vP);

      // Display Basis
      Basis basis = getPolarBasis(pointR, pointT);
      vec2 probepoint{pointR * -std::cos(pointT), pointR * std::sin(pointT)};
      vec2 rEndpoint{basis.e1.x + probepoint.x, basis.e1.y + probepoint.y};
      vec2 tEndpoint{basis.e2.x + probepoint.x, basis.e2.y + probepoint.y};
      vec2 probeOnScreen =
          TransformToScreenCoords(probepoint.x, probepoint.y, windowPos);
      vec2 rPointOnScreen =
          TransformToScreenCoords(rEndpoint.x, rEndpoint.y, windowPos);
      vec2 tPointOnScreen =
          TransformToScreenCoords(tEndpoint.x, tEndpoint.y, windowPos);
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RED
      SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y,
                         rPointOnScreen.x, rPointOnScreen.y);
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // GREEN
      SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y,
                         tPointOnScreen.x, tPointOnScreen.y);

      // RK4
      vec8 k1 = getYPrime(y[i]);
      vec8 k2 = getYPrime(y[i] + k1 * (timeStep / 2));
      vec8 k3 = getYPrime(y[i] + k2 * (timeStep / 2));
      vec8 k4 = getYPrime(y[i] + k3 * timeStep);
      y[i] = y[i] + (k1 + (k2 * 2) + (k3 * 2) + k4) * (timeStep / 6);

      // Add point to Trajectory
      vec2 cartesianPosition = CartesianTransformaion(pointR, pointT);
      trajectory[i].push_back(cartesianPosition);

      // Draw Trajectory
      SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // BLUE
      for (vec2 point : trajectory[i]) {
        vec2 screenPoint = TransformToScreenCoords(point.x, point.y, windowPos);
        SDL_RenderDrawPoint(renderer, screenPoint.x, screenPoint.y);
      }

      // Iterative Normalisation
      double velocityMagnitude =
          std::sqrt(y[i].y2 * y[i].y2 + y[i].y4 * y[i].y4 + y[i].y6 * y[i].y6 +
                    y[i].y8 * y[i].y8) != 0
              ? std::sqrt(y[i].y2 * y[i].y2 + y[i].y4 * y[i].y4 +
                          y[i].y6 * y[i].y6 + y[i].y8 * y[i].y8)
              : 1;
      y[i].y2 = (y[i].y2 / velocityMagnitude) * c;
      y[i].y4 = (y[i].y4 / velocityMagnitude) * c;
      y[i].y6 = (y[i].y6 / velocityMagnitude) * c;
      y[i].y8 = (y[i].y8 / velocityMagnitude) * c;

      // Teleport
      if (tp) {
        vec2 cartPos = CartesianTransformaion(y[i].y1, y[i].y7);
        if (y[i].y1 <= 1 && false) {
          t = timeSpan + 1;
        }
        if (y[i].y1 >= boundary) {
          y[i].y1 = boundary - 1;
          y[i].y7 = PolarTransformation(-cartPos.x, -cartPos.y).y;
          y[i].y2 = -y[i].y2;
          y[i].y8 = -y[i].y8;
        }
      }

      // Text
      if (text && i == 0) {
        TTF_Init();
        TTF_Font *Sans =
            TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
        SDL_Color Gray = {255, 255, 255};
        std::string out = "V-Time: " + std::to_string(y[i].y4) +
                          " V-Radial: " + std::to_string(y[i].y2) +
                          " V-Azimuth: " + std::to_string(y[i].y8) +
                          " V-Polar: " + std::to_string(y[i].y6) +
                          " V-Total: " + std::to_string(velocity);
        SDL_Surface *surfaceMessage =
            TTF_RenderText_Solid(Sans, out.c_str(), Gray);
        SDL_Texture *Message =
            SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_Rect Message_rect;
        Message_rect.w = 1000;
        Message_rect.h = 2 * 24;
        Message_rect.x = SCREEN_WIDTH - Message_rect.w;
        Message_rect.y = SCREEN_HEIGHT - Message_rect.h;
        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
        TTF_CloseFont(Sans);
        TTF_Quit();
      }
    }

    // SDL stuff
    SDL_RenderPresent(renderer);
    SDL_Delay(1000 * timeStep);
  }

  // Do SDL stuff
  SDL_RenderPresent(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;
}
