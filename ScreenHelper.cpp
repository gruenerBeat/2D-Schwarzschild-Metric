#include "ScreenHelper.h"
#include "GeometryHelper.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCALE 50

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