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

/*vec2 CalcPolarPositionFormBasis(Basis b) {
  double theta = getAngle(b.e1.x, b.e1.y);
  double r = std::sqrt(b.e2.x * b.e2.x + b.e2.y * b.e2.y);
  vec2 pos{r, theta};
  return pos;
}*/

/*TransformingVector PolarToOrthonormalBasis(TransformingVector v) {
  vec2 polarPos = CalcPolarPositionFormBasis(v.basis);
  vec2 ortho1{1, 0};
  vec2 ortho2{0, 2};
  Basis orthoBasis{ortho1, ortho2};
  double compX = v.components.x * -std::cos(polarPos.y) + v.components.y * polarPos.x * std::sin(polarPos.y);
  double compY = v.components.x * std::sin(polarPos.y) + v.components.y * polarPos.x * std::cos(polarPos.y);
  vec2 components{compX, compY};
  TransformingVector vector{orthoBasis, components};
  return vector;
}*/

/*Basis getPolarBasis(double r, double theta) {
  Basis basis;
  basis.e1.x = -std::cos(theta);
  basis.e1.y = std::sin(theta);
  basis.e2.x = r * std::cos(M_PI - (theta + M_PI_2));
  basis.e2.y = r * std::sin(M_PI - (theta + M_PI_2));
  return basis;
}*/

vec3 CartesianTransformaion(vec3 coords) {
  vec3 cartesianCoords{coords.x * std::cos(coords.z) * std::sin(coords.y), coords.x * std::sin(coords.z) * std::sin(coords.y), coords.x * std::cos(coords.y)};
  return cartesianCoords;
}

vec3 PolarTransformation(vec3 coords) {
  double r = std::sqrt(coords.x * coords.x + coords.y * coords.y + coords.z * coords.z);
  vec3 polarCoords{r, getAngle(coords.x, coords.y), std::acos(coords.z / r)};
  return polarCoords;
}

/*TransformingVector UpdateBasis(TransformingVector v, double t, double r) {
  Basis oldBasis = v.basis;
  Basis newBasis = getPolarBasis(r, t);
  vec2 conTraE1{newBasis.e2.y, -newBasis.e1.y};
  vec2 conTraE2{-newBasis.e2.x, newBasis.e1.x};
  Basis conjugateTranspose{conTraE1, conTraE2};
  double invDeterminant = 1 / (newBasis.e1.x * newBasis.e2.y - newBasis.e2.x * newBasis.e1.y);
  Basis inverseBasis{vec2{invDeterminant * conjugateTranspose.e1.x, invDeterminant * conjugateTranspose.e1.y}, vec2{invDeterminant * conjugateTranspose.e2.x, invDeterminant * conjugateTranspose.e2.y}};
  double t1 = inverseBasis.e1.x * oldBasis.e1.x + inverseBasis.e2.x * oldBasis.e1.y;
  double t2 = inverseBasis.e1.y * oldBasis.e1.x + inverseBasis.e2.y * oldBasis.e1.y;
  double t3 = inverseBasis.e1.x * oldBasis.e2.x + inverseBasis.e2.x * oldBasis.e2.y;
  double t4 = inverseBasis.e1.y * oldBasis.e2.x + inverseBasis.e2.y * oldBasis.e2.y;
  Basis transformation{vec2{t1, t2}, vec2{t3, t4}};
  double xOld = v.components.x;
  double yOld = v.components.y;
  v.components.x = transformation.e1.x * xOld + transformation.e2.x * yOld;
  v.components.y = transformation.e1.y * xOld + transformation.e2.y * yOld;
  v.basis = newBasis;
  return v;
}*/

double cot(double x) {
  return x == 0 ? 100000 : 1 / std::tan(x);
}