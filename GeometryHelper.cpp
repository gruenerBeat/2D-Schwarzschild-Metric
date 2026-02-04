#include "GeometryHelper.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

Basis getBasis(vec2 position) {
  return Basis{vec2{1, 0}, vec2{0, 1}};
}
