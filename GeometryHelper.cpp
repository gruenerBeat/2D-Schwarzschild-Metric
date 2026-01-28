
#include "GeometryHelper.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include "Metric.h"
#include <cmath>

vec3 Normalize(vec3 a) {
  double magnitude = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
  return magnitude != 0
             ? vec3{a.x / magnitude, a.y / magnitude, a.z / magnitude}
             : vec3{0, 0, 0};
}

double getAngle(double x, double y) {
  if (x == 0 && y == 0) {
    return 0;
  }
  return std::atan2(y, -x);
}

vec3 CartesianTransformaion(vec3 coords) {
  vec3 cartesianCoords{coords.x * std::cos(coords.z) * std::sin(coords.y),
                       coords.x * std::sin(coords.z) * std::sin(coords.y),
                       coords.x * std::cos(coords.y)};
  return cartesianCoords;
}

vec3 PolarTransformation(vec3 coords) {
  double r = std::sqrt(coords.x * coords.x + coords.y * coords.y +
                       coords.z * coords.z);
  vec3 polarCoords{r, std::acos(coords.y / r), getAngle(coords.x, coords.z)};
  return polarCoords;
}

vec3 Cross(vec3 a, vec3 b) {
  return vec3{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x};
}

double Dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

double cot(double x) { return x == 0 ? 100000 : 1 / std::tan(x); }

vec4 ToVec4(vec3 a, double b) { return vec4{a.x, a.y, a.z, b}; }

vec3 ToVec3(vec4 a) { return vec3{a.x, a.y, a.z}; }

vec3 SphericalTransformationAt(vec3 polarCoords, vec3 vector) {
  double r = polarCoords.x;
  double t = polarCoords.y;
  double p = polarCoords.z;
  return vec3{vector.x * std::sin(t) * std::cos(p) +
                  vector.z * std::sin(t) * std::sin(p) + vector.y * std::cos(t),
              (vector.x * std::cos(t) * std::cos(p) +
               vector.z * std::cos(t) * std::sin(p) + vector.y * std::sin(t)) /
                  r,
              -(vector.x * std::sin(p) - vector.z * std::cos(p)) / r};
}

double Dot4(vec4 a, vec4 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Matrix4x4 Inverse(Matrix4x4 a) {
  double determinant =
      a.Row1.y * a.Row2.w * a.Row3.z * a.Row4.x -
      a.Row1.y * a.Row2.z * a.Row3.w * a.Row4.x -
      a.Row1.x * a.Row2.w * a.Row3.z * a.Row4.y +
      a.Row1.x * a.Row2.z * a.Row3.w * a.Row4.y -
      a.Row1.y * a.Row2.w * a.Row3.x * a.Row4.z +
      a.Row1.x * a.Row2.w * a.Row3.y * a.Row4.z + a.Row1.y * a.Row2.x +
      a.Row3.w * a.Row4.z - a.Row1.x * a.Row2.y * a.Row3.w * a.Row4.z +
      a.Row1.w * (a.Row2.z * (a.Row3.y * a.Row4.x - a.Row3.x * a.Row4.y) +
                  a.Row2.y * (a.Row3.x * a.Row4.z - a.Row3.z * a.Row4.x) +
                  a.Row2.x * (a.Row3.z * a.Row4.y - a.Row3.y * a.Row4.z)) +
      a.Row1.y * a.Row2.z * a.Row3.x * a.Row4.w -
      a.Row1.x * a.Row2.z * a.Row3.y * a.Row4.w -
      a.Row1.y * a.Row2.x * a.Row3.z * a.Row4.w +
      a.Row1.x * a.Row2.y * a.Row3.z * a.Row4.w +
      a.Row1.z * (a.Row2.w * (a.Row3.x * a.Row4.y - a.Row3.y * a.Row4.x) +
                  a.Row2.y * (a.Row3.w * a.Row4.x - a.Row3.x * a.Row4.w) +
                  a.Row2.x * (a.Row3.y * a.Row4.w - a.Row3.w * a.Row4.y));
  Matrix4x4 out{
      vec4{-a.Row2.w * a.Row3.z * a.Row4.y + a.Row2.z * a.Row3.w * a.Row4.y +
               a.Row2.w * a.Row3.y * a.Row4.z - a.Row2.y * a.Row3.w * a.Row4.z -
               a.Row2.z * a.Row3.y * a.Row4.w + a.Row2.y * a.Row3.z * a.Row4.w,
           a.Row1.w * a.Row3.z * a.Row4.y - a.Row1.z * a.Row3.w * a.Row4.y -
               a.Row1.w * a.Row3.y * a.Row4.z + a.Row1.y * a.Row3.w * a.Row4.z +
               a.Row1.z * a.Row3.y * a.Row4.w - a.Row1.y * a.Row3.z * a.Row4.w,
           -a.Row1.w * a.Row2.z * a.Row4.y + a.Row1.z * a.Row2.w * a.Row4.y +
               a.Row1.w * a.Row2.y * a.Row4.z - a.Row1.y * a.Row2.w * a.Row4.z -
               a.Row1.z * a.Row2.y * a.Row4.w + a.Row1.y * a.Row2.z * a.Row4.w,
           a.Row1.w * a.Row2.z * a.Row3.y - a.Row1.z * a.Row2.w * a.Row3.y -
               a.Row1.w * a.Row2.y * a.Row3.z + a.Row1.y * a.Row2.w * a.Row3.z +
               a.Row1.z * a.Row2.y * a.Row3.w - a.Row1.y * a.Row2.z * a.Row3.w},
      vec4{a.Row2.w * a.Row3.z * a.Row4.x - a.Row2.z * a.Row3.w * a.Row4.x -
               a.Row2.w * a.Row3.x * a.Row4.z + a.Row2.x * a.Row3.w * a.Row4.z +
               a.Row2.z * a.Row3.x * a.Row4.w - a.Row2.x * a.Row3.z * a.Row4.w,
           -a.Row1.w * a.Row3.z * a.Row4.x + a.Row1.z * a.Row3.w * a.Row4.x +
               a.Row1.w * a.Row3.x * a.Row4.z - a.Row1.x * a.Row3.w * a.Row4.z -
               a.Row1.z * a.Row3.x * a.Row4.w + a.Row1.x * a.Row3.z * a.Row4.w,
           a.Row1.w * a.Row2.z * a.Row4.x - a.Row1.z * a.Row2.w * a.Row4.x -
               a.Row1.w * a.Row2.x * a.Row4.z + a.Row1.x * a.Row2.w * a.Row4.z +
               a.Row1.z * a.Row2.x * a.Row4.w - a.Row1.x * a.Row2.z * a.Row4.w,
           -a.Row1.w * a.Row2.z * a.Row3.x + a.Row1.z * a.Row2.w * a.Row3.x +
               a.Row1.w * a.Row2.x * a.Row3.z - a.Row1.x * a.Row2.w * a.Row3.z -
               a.Row1.z * a.Row2.x * a.Row3.w + a.Row1.x * a.Row2.z * a.Row3.w},
      vec4{-a.Row2.w * a.Row3.y * a.Row4.x + a.Row2.y * a.Row3.w * a.Row4.x +
               a.Row2.w * a.Row3.x * a.Row4.y - a.Row2.x * a.Row3.w * a.Row4.y -
               a.Row2.y * a.Row3.x * a.Row4.w + a.Row2.x * a.Row3.y * a.Row4.w,
           a.Row1.w * a.Row3.y * a.Row4.x - a.Row1.y * a.Row3.w * a.Row4.x -
               a.Row1.w * a.Row3.x * a.Row4.y + a.Row1.x * a.Row3.w * a.Row4.y +
               a.Row1.y * a.Row3.x * a.Row4.w - a.Row1.x * a.Row3.y * a.Row4.w,
           -a.Row1.w * a.Row2.y * a.Row4.x + a.Row1.y * a.Row2.w * a.Row4.x +
               a.Row1.w * a.Row2.x * a.Row4.y - a.Row1.x * a.Row2.w * a.Row4.y -
               a.Row1.y * a.Row2.x * a.Row4.w + a.Row1.x * a.Row2.y * a.Row4.w,
           a.Row1.w * a.Row2.y * a.Row3.x - a.Row1.y * a.Row2.w * a.Row3.x -
               a.Row1.w * a.Row2.x * a.Row3.y + a.Row1.x * a.Row2.w * a.Row3.y +
               a.Row1.y * a.Row2.x * a.Row3.w - a.Row1.x * a.Row2.y * a.Row3.w},
      vec4{a.Row2.z * a.Row3.y * a.Row4.x - a.Row2.y * a.Row3.z * a.Row4.x -
               a.Row2.z * a.Row3.x * a.Row4.y + a.Row2.x * a.Row3.z * a.Row4.y +
               a.Row2.y * a.Row3.x * a.Row4.z - a.Row2.x * a.Row3.y * a.Row4.z,
           -a.Row1.z * a.Row3.y * a.Row4.x + a.Row1.y * a.Row3.z * a.Row4.x +
               a.Row1.z * a.Row3.x * a.Row4.y - a.Row1.x * a.Row3.z * a.Row4.y -
               a.Row1.y * a.Row3.x * a.Row4.z + a.Row1.x * a.Row3.y * a.Row4.z,
           a.Row1.z * a.Row2.y * a.Row4.x - a.Row1.y * a.Row2.z * a.Row4.x -
               a.Row1.z * a.Row2.x * a.Row4.y + a.Row1.x * a.Row2.z * a.Row4.y +
               a.Row1.y * a.Row2.x * a.Row4.z - a.Row1.x * a.Row2.y * a.Row4.z,
           -a.Row1.z * a.Row2.y * a.Row3.x + a.Row1.y * a.Row2.z * a.Row3.x +
               a.Row1.z * a.Row2.x * a.Row3.y - a.Row1.x * a.Row2.z * a.Row3.y -
               a.Row1.y * a.Row2.x * a.Row3.z +
               a.Row1.x * a.Row2.y * a.Row3.z}};
  return out * (1 / determinant);
}
