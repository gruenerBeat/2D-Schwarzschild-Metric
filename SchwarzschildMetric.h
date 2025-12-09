#ifndef SCHWARZSCHILDMETRIC_H
#define SCHWARZSCHILDMETRIC_H

double Christoffel(int index1, int index2, int index3);
TransformingVector GetAcceleration(TransformingVector velocity, double r, double t);

#endif