#include <stdio.h>
#include <stdlib.h>

#include "RenderingUtils.h"
#include "GeometryHelper.h"

#define _USE_MATH_DEFINES
#include <cmath>

vec3 ToNDC(int x, int y, int width, int height) {
    double ndcX = (x / width) * 2.0 - 1.0;
    double ndcY = (y / height) * 2.0 - 1.0;
    ndcX *= (width / height);
    return vec3{ndcX, 0, ndcY};
}