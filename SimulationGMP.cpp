#include "mpreal.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCALE 25
#define THICKNESS 0.98
#define boundary 30
#define TOLERANCE 1e-8

bool text = false;
bool tp = false;
bool cordGrid = false;

#include "Metric.h"
#include "ScreenHelper.h"
#include "math/MatrixGMP.hpp"
#include "math/Polar.hpp"
#include "math/VectorGMP.hpp"

VectorGMP<8> y[100];

/*VectorGMP<8> y[] = {
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0025}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.005}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0075}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.01}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0125}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.015}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0175}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.02}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0225}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.025}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0275}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.03}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0325}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.035}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0375}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.04}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0425}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.045}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.0475}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, 0.05}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0025}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.005}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0075}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.01}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0125}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.015}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0175}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.02}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0225}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.025}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0275}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.03}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0325}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.035}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0375}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.04}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0425}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.045}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.0475}),
    VectorGMP<8>({10, -1, 0, 0, M_PI_2, 0, 0, -0.05}),
    VectorGMP<8>({1.5 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1}),
    VectorGMP<8>({1.25 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1}),
    VectorGMP<8>({1.05 * rs, 0, 0, 0, M_PI_2, 0, M_PI_2, 1}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.1 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.2 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.3 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.4 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.5 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.6 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.7 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.8 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 0.9 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.0 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.1 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.2 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.3 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.4 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.5 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.6 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.7 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.8 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 1.9 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.0 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.1 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.2 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.3 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.4 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.5 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.6 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.7 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.8 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 2.9 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.0 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.1 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.2 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.3 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.4 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.5 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.6 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.7 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.8 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 3.9 * M_PI_2, 0.9}),
    VectorGMP<8>({20, -0.1, 0, 0, M_PI_2, 0, 4.0 * M_PI_2, 0.9}),

};*/

constexpr int pointSize = sizeof(y) / sizeof(y[0]);

constexpr double timeSpan = 100;
constexpr double timeStep = 0.005;
constexpr int maxSteps = 100;

constexpr int movementRate = 2;

VectorGMP<3> variant1(VectorGMP<3> y) { return y + timeStep * getYPrime(y); }

VectorGMP<3> variant2(VectorGMP<3> y) {

  mpfr::mpreal vals[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  MatrixGMP<3, 3> id3x3(vals);

  VectorGMP<3> y0 = y;
  VectorGMP<3> y1 = y;
  VectorGMP<3> y2 = y;

  for (int i = 0; i < maxSteps; i++) {

    VectorGMP<3> phi = y1 - y0 - timeStep * getYPrime((y1 + y0) * 0.5);
    // printf("%d 0.5 * (yNew + y): %s \n", i,
    //        (0.5 * y1 + 0.5 * y0).toString().c_str());
    // std::cout << "MAG: " << phi.magnitude() << std::endl;
    if (phi.magnitude() < TOLERANCE) {
      break;
    }
    MatrixGMP<3, 3> phi_ =
        id3x3 - timeStep * 0.5 * getYPrimePrime((y1 + y0) * 0.5);
    // printf("%d PHI: %s \n", i, phi.toString().c_str());
    // printf("%d PHIPrime: %s \n", i, phi_.toString().c_str());
    // printf("%d Inv * phi_: %s \n", i, (inv(phi_) * phi_).toString().c_str());
    y2 = y1 - inv(phi_) * phi;
    // printf("%d y: %s \n\n", i, y2.toString().c_str());
    // printf("%d yNew-y: %s \n\n", i, (y1 - y0).toString().c_str());
    y0 = y1;
    y1 = y2;
  }
  y = y2;
  return y;
}

int main() {
  mpfr::mpreal::set_default_prec(1024);

  VectorGMP<2> commonVelocity = VectorGMP<2>({-1, 0});
  for (int i = 0; i < 100; i++) {
    double x = 10;
    double yVal =
        i * SCREEN_HEIGHT / (100.0 * SCALE) - (SCREEN_HEIGHT / (2.0 * SCALE));
    VectorGMP<2> pointCartesian = VectorGMP<2>({x, yVal});
    VectorGMP<2> pointPolar = cartesianToPolar(VectorGMP<2>({x, yVal}));
    VectorGMP<2> polarVelocity =
        VectorGMP<2>({commonVelocity(0) * mpfr::cos(pointPolar(1)) +
                          commonVelocity(1) * mpfr::sin(pointPolar(1)),
                      -(commonVelocity(0) * mpfr::sin(pointPolar(1)) -
                        commonVelocity(1) * mpfr::cos(pointPolar(1))) /
                          pointPolar(0)});
    y[i] = VectorGMP<8>({pointPolar(0), polarVelocity(0), 0, 0, M_PI_2, 0,
                         pointPolar(1), polarVelocity(1)});
  }
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
  Vector<2> windowPos({0, 0});

  for (int i = 0; i < pointSize; i++) {
    // Initial Normalisation
    mpfr::mpreal velocityMagnitude =
        mpfr::sqrt(y[i](1) * y[i](1) + y[i](3) * y[i](3) + y[i](5) * y[i](5) +
                   y[i](7) * y[i](7)) != 0
            ? mpfr::sqrt(y[i](1) * y[i](1) + y[i](3) * y[i](3) +
                         y[i](5) * y[i](5) + y[i](7) * y[i](7))
            : 1;
    y[i](1) = (y[i](1) / velocityMagnitude);
    y[i](3) = (y[i](3) / velocityMagnitude);

    y[i](5) = (y[i](5) / velocityMagnitude);
    y[i](7) = (y[i](7) / velocityMagnitude);
  }

  std::list<VectorGMP<2>> trajectory[pointSize];
  bool sim[pointSize];
  for (int i = 0; i < pointSize; i++) {
    sim[i] = true;
  }

  for (int t = 0; t <= timeSpan; t += timeStep) {
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
          windowPos(1) += movementRate;
        }
        if (down) {
          windowPos(1) -= movementRate;
        }
        if (left) {
          windowPos(0) += movementRate;
        }
        if (right) {
          windowPos(0) -= movementRate;
        }
      }
    } while (paused);

    // Draw Coordinate Grid
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      for (int y = 0; y < SCREEN_HEIGHT; y++) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // WHITE
        Vector<2> pos = TransformToSimulationCoords(x, y, windowPos);
        Vector<2> polarPosition = cartesianToPolar(Vector<2>({pos(0), pos(1)}));
        double r = polarPosition(0);
        double theta = polarPosition(1);
        if (std::fmod(r, 1) >= THICKNESS && cordGrid) {
          SDL_RenderDrawPoint(renderer, x, y);
        }
        if (r >= rs && r <= rs + THICKNESS / SCALE) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 255,
                                 255); // PURPLE
          SDL_RenderDrawPoint(renderer, x, y);
        }
        if (r >= 1.5 * rs && r <= 1.5 * rs + THICKNESS / (2.0 * SCALE)) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
          SDL_RenderDrawPoint(renderer, x, y);
        }
      }
    }

    if (cordGrid) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // WHITE
      Vector<2> originOnScreen = TransformToScreenCoords(0, 0, windowPos);
      for (int i = 0; i < 8; i++) {
        int size = 500000;
        Vector<2> screenEndPoint =
            TransformToScreenCoords(size * std::cos(i * M_PI_4),
                                    size * std::sin(i * M_PI_4), windowPos);
        SDL_RenderDrawLine(renderer, (double)originOnScreen(0),
                           (double)originOnScreen(1), (double)screenEndPoint(0),
                           (double)screenEndPoint(1));
      }
    }
    for (int i = 0; i < pointSize; i++) {

      // Velocity
      mpfr::mpreal vT = y[i](3);
      mpfr::mpreal vR = y[i](1);
      mpfr::mpreal vA = y[i](7);
      mpfr::mpreal vP = y[i](5);

      mpfr::mpreal pointR = y[i](0);
      mpfr::mpreal pointT = y[i](6);

      mpfr::mpreal velocity = mpfr::sqrt(vT * vT + vR * vR + vA * vA + vP * vP);

      if (pointR < rs + TOLERANCE) {

        sim[i] = false;
      }

      // Display Basis
      /*Basis basis = getPolarBasis(pointR, pointT);
      VectorGMP<2> probepoint{pointR * -std::cos(pointT), pointR *
      std::sin(pointT)}; VectorGMP<2> rEndpoint{basis.e1.x + probepoint.x,
      basis.e1.y + probepoint.y}; VectorGMP<2> tEndpoint{basis.e2.x +
      probepoint.x, basis.e2.y + probepoint.y}; VectorGMP<2> probeOnScreen =
          TransformToScreenCoords(probepoint.x, probepoint.y, windowPos);
      VectorGMP<2> rPointOnScreen =
          TransformToScreenCoords(rEndpoint.x, rEndpoint.y, windowPos);
      VectorGMP<2> tPointOnScreen =
          TransformToScreenCoords(tEndpoint.x, tEndpoint.y, windowPos);
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RED
      SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y,
                         rPointOnScreen.x, rPointOnScreen.y);
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // GREEN
      SDL_RenderDrawLine(renderer, probeOnScreen.x, probeOnScreen.y,
                         tPointOnScreen.x, tPointOnScreen.y);*/

      if (sim[i]) {
        VectorGMP<3> y3 = VectorGMP<3>({y[i](0), y[i](1), y[i](7)});
        y3 = variant1(y3);
        y[i](0) = y3(0);
        y[i](1) = y3(1);
        y[i](6) += timeStep * y3(2);
        y[i](7) = y3(2);

        // Add point to Trajectory
        VectorGMP<2> cartesianPosition =
            polarToCartesian(VectorGMP<2>({pointR, pointT}));
        trajectory[i].push_back(cartesianPosition);
      }
      // Draw Trajectory
      SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // BLUE
      for (VectorGMP<2> point : trajectory[i]) {
        Vector<2> screenPoint = TransformToScreenCoords(
            (double)point(0), (double)point(1), windowPos);
        SDL_RenderDrawPoint(renderer, (double)screenPoint(0),
                            (double)screenPoint(1));
      }

      // Iterative Normalisation
      mpfr::mpreal velocityMagnitude =
          mpfr::sqrt(y[i](1) * y[i](1) + y[i](3) * y[i](3) + y[i](5) * y[i](5) +
                     y[i](7) * y[i](7)) != 0
              ? mpfr::sqrt(y[i](1) * y[i](1) + y[i](3) * y[i](3) +
                           y[i](5) * y[i](5) + y[i](7) * y[i](7))
              : 1;
      y[i](1) = (y[i](1) / velocityMagnitude);
      y[i](3) = (y[i](3) / velocityMagnitude);
      y[i](5) = (y[i](5) / velocityMagnitude);
      y[i](7) = (y[i](7) / velocityMagnitude);

      // Teleport
      if (tp) {
        VectorGMP<2> cartPos =
            polarToCartesian(VectorGMP<2>({y[i](0), y[i](6)}));
        if (y[i](0) <= 1 && false) {
          t = timeSpan + 1;
        }
        if (y[i](0) >= boundary) {
          y[i](0) = boundary - 1;
          y[i](6) =
              cartesianToPolar(VectorGMP<2>({-cartPos(1), -cartPos(2)}))(1);
          y[i](1) = -y[i](1);
          y[i](7) = -y[i](7);
        }
      }

      // Text
      if (text && i == 0) {
        TTF_Init();
        TTF_Font *Sans =
            TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
        SDL_Color Gray = {255, 255, 255};
        std::string out = "V-Time: " + y[i](3).toString() +
                          " V-Radial: " + y[i](1).toString() +
                          " V-Azimuth: " + y[i](7).toString() +
                          " V-Polar: " + y[i](5).toString() +
                          " V-Total: " + velocity.toString();
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
