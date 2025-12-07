#include "GeometryHelper.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

TransformingVector PolarToOrthonormalBasis(TransformingVector v, double r, double theta) {
    vec2 ortho1{1, 0};
    vec2 ortho2{0, 2};
    Basis orthoBasis{ortho1, ortho2};
    double compX = v.components.x * std::cos(theta) + v.components.y * r * std::sin(theta);
    double compY = v.components.x * std::sin(theta) + v.components.y * r * std::cos(theta);
    vec2 components{compX, compY};
    TransformingVector vector{orthoBasis, components};
    return vector;
}

double getAngle(double x, double y) {
  if(x == 0 && y == 0) {
    return 0;
  }
  return std::atan2(y, -x);
}

Basis getPolarBasis(double r, double theta) {
  Basis basis;
  basis.e1.x = -std::cos(theta);
  basis.e1.y = std::sin(theta);
  basis.e2.x = r * std::cos(M_PI - (theta + M_PI_2));
  basis.e2.y = r * std::sin(M_PI - (theta + M_PI_2));
  return basis;
}

vec2 CartesianTransformaion(float r, float t) {
  vec2 coords{r * -std::cos(t), r * std::sin(t)};
  return coords;
}