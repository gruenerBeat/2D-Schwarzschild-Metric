#ifndef METRIC_H
#define METRIC_H

#include "GeometryHelper.h"

constexpr double M = 2;
constexpr double c = 2;
constexpr double G = 1;
constexpr double rs = (2 * G * M) / (c * c);

vec8 getYPrime(vec8 y);

#endif