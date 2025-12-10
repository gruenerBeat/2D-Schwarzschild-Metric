#ifndef METRIC_H
#define METRIC_H

#include "GeometryHelper.h"

double Christoffel(int index1, int index2, int index3);
TransformingVector GetAcceleration(TransformingVector velocity, double r, double t);

#endif