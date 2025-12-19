#include "Metric.h"
#include "GeometryHelper.h"
#include <stdio.h>
#include <stdlib.h>

constexpr double M = 2;

#define _USE_MATH_DEFINES
#include <cmath>

vec8 getYPrime(vec8 y) {
    double a = y.y1 - 2 * M;
    return vec8{/*y.y2, -(M * a / (y.y1 * y.y1 * y.y1)) * y.y4 * y.y4 + M / (y.y1 * a) * y.y2 * y.y2 + a * y.y6 * y.y6 + a * std::sin(y.y5) * std::sin(y.y5) * y.y8 * y.y8,
                y.y4, -2 * M / (a * y.y1) * y.y2 * y.y4,
                y.y6, -2 / y.y1 * y.y2 * y.y6 + std::sin(y.y5) * std::cos(y.y5) * y.y8 * y.y8,
                y.y8, -2 / y.y1 * y.y2 * y.y8 - 2 * cot(y.y5) * y.y6 * y.y8*/

                /*y.y2, 0,
                y.y4, 0,
                y.y6, 0,
                y.y8, 0*/

                y.y2, y.y1 * y.y8,
                y.y4, 0,
                y.y6, 0,
                y.y8, (2 / y.y1) * y.y2 * y.y8
            };
}