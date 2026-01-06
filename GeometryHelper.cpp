#include "GeometryHelper.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include "Metric.h"

vec3 Normalize(vec3 a) {
  double magnitude = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
  return magnitude != 0 ? vec3{a.x / magnitude, a.y  / magnitude, a.z / magnitude} : vec3{0, 0, 0};
}

double getAngle(double x, double y) {
  if(x == 0 && y == 0) {
    return 0;
  }
  return std::atan2(y, -x);
}

vec3 CartesianTransformaion(vec3 coords) {
  vec3 cartesianCoords{coords.x * std::cos(coords.z) * std::sin(coords.y), coords.x * std::sin(coords.z) * std::sin(coords.y), coords.x * std::cos(coords.y)};
  return cartesianCoords;
}

vec3 PolarTransformation(vec3 coords) {
  double r = std::sqrt(coords.x * coords.x + coords.y * coords.y + coords.z * coords.z);
  vec3 polarCoords{r, std::acos(coords.y / r), getAngle(coords.x, coords.z)};
  return polarCoords;
}

vec3 Cross(vec3 a, vec3 b) {
  return vec3{
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}

double Dot(vec3 a, vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

double cot(double x) {
  return x == 0 ? 100000 : 1 / std::tan(x);
}

vec4 ToVec4(vec3 a, double b) {
  return vec4{
    a.x,
    a.y,
    a.z,
    b
  };
}

vec3 ToVec3(vec4 a) {
  return vec3{
    a.x,
    a.y,
    a.z
  };
}

vec3 PolarTransformationAt(vec3 polarCoords, vec3 vector) {
  double r = polarCoords.x;
  double t = polarCoords.y;
  double p = polarCoords.z;
  return vec3{
    vector.x * std::sin(t) * std::cos(p) + vector.z * std::sin(t) * std::sin(p) + vector.y * std::cos(t),
    (vector.x * std::cos(t) * std::cos(p) + vector.z * std::cos(t) * std::sin(p) + vector.y * std::sin(t)) / r,
    -(vector.x * std::sin(p) - vector.z * std::cos(p)) / r
  };
}