#include <stdio.h>
#include <stdlib.h>

#include "GeometryHelper.h"
#include "RenderingUtils.h"

#define _USE_MATH_DEFINES
#include <cmath>

vec3 ToNDC(int x, int y, int width, int height) {
  printf("x=%d, y=%d", x, y);
  double ndcX = (x / (double)width) * 2.0 - 1.0;
  double ndcY = (y / (double)height) * 2.0 - 1.0;
  ndcX *= (width / (double)height);
  printf("lx=%lf, ly=%lf", ndcX, ndcY);
  return vec3{ndcX, 0, ndcY};
}
