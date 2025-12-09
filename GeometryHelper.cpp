#include "GeometryHelper.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

double getAngle(double x, double y) {
  if(x == 0 && y == 0) {
    return 0;
  }
  return std::atan2(y, -x);
}

vec2 CalcPolarPositionFormBasis(Basis b) {
  double theta = getAngle(b.e1.x, b.e1.y);
  double r = std::sqrt(b.e2.x * b.e2.x + b.e2.y * b.e2.y);
  vec2 pos{r, theta};
  return pos;
}

TransformingVector PolarToOrthonormalBasis(TransformingVector v) {
  vec2 polarPos = CalcPolarPositionFormBasis(v.basis);
  vec2 ortho1{1, 0};
  vec2 ortho2{0, 2};
  Basis orthoBasis{ortho1, ortho2};
  double compX = v.components.x * -std::cos(polarPos.y) + v.components.y * polarPos.x * std::sin(polarPos.y);
  double compY = v.components.x * std::sin(polarPos.y) + v.components.y * polarPos.x * std::cos(polarPos.y);
  vec2 components{compX, compY};
  TransformingVector vector{orthoBasis, components};
  return vector;
}

Basis getPolarBasis(double r, double theta) {
  Basis basis;
  basis.e1.x = -std::cos(theta);
  basis.e1.y = std::sin(theta);
  basis.e2.x = r * std::cos(M_PI - (theta + M_PI_2));
  basis.e2.y = r * std::sin(M_PI - (theta + M_PI_2));
  return basis;
}

vec2 CartesianTransformaion(double r, double t) {
  vec2 coords{r * -std::cos(t), r * std::sin(t)};
  return coords;
}

vec2 PolarTransformation(double x, double y) {
  vec2 polarCoords{std::sqrt(x * x + y * y), getAngle(x, y)};
  return polarCoords;
}

TransformingVector UpdateBasis(TransformingVector v, double t, double r) {
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
}