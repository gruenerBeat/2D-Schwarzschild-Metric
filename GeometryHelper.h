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

TransformingVector PolarToOrthonormalBasis(TransformingVector v);
double getAngle(double x, double y);
Basis getPolarBasis(double r, double theta);
vec2 CartesianTransformaion(double r, double t);
vec2 PolarTransformation(double x, double y);
TransformingVector UpdateBasis(TransformingVector v, double t, double r);
vec2 CalcPolarPositionFormBasis(Basis b);
double cot(double x);

#endif