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


//Funktioniert nicht!
TransformingVector UpdateBasis(TransformingVector v, double t, double r, double time) {
  Basis newBasis = getPolarBasis(r, t);
  vec2 changeRR{(newBasis.e1.x - v.basis.e1.x) / time, (newBasis.e1.y - v.basis.e1.y) / time};
  vec2 changeRT{(newBasis.e1.x - v.basis.e2.x) / time, (newBasis.e1.y - v.basis.e2.y) / time};
  vec2 changeTR{(newBasis.e2.x - v.basis.e1.x) / time, (newBasis.e2.y - v.basis.e1.y) / time};
  vec2 changeTT{(newBasis.e2.x - v.basis.e2.x) / time, (newBasis.e2.y - v.basis.e2.y) / time};
  vec2 rChange{r * changeRR.x + t * changeRT.x, r * changeRR.y + t * changeRT.y};
  vec2 tChange{r * changeTR.x + t * changeTT.x, r * changeTR.y + t * changeTT.y};
  v.components.x = std::sqrt(rChange.x * rChange.x + rChange.y * rChange.y);
  v.components.y = std::sqrt(tChange.x * tChange.x + tChange.y * tChange.y);
  v.basis = newBasis;
  return v;
}