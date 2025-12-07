#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

struct vec2 {
    double x;
    double y;
};

struct Basis {
    vec2 e1;
    vec2 e2;
};

struct TransformingVector {
    Basis basis;
    vec2 components;
};

TransformingVector PolarToOrthonormalBasis(TransformingVector v, double r, double theta);
double getAngle(double x, double y);
Basis getPolarBasis(double r, double theta);
vec2 CartesianTransformaion(float r, float t);

#endif