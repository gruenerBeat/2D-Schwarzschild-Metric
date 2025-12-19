#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

struct vec2 {
    double x;
    double y;

    vec2 operator+(vec2 a) {
        return vec2{
            a.x + x,
            a.y + y
        };
    };

    vec2 operator-(vec2 a) {
        return vec2{
            a.x + x,
            a.y + y
        };
    }
    vec2 operator*(double a) {
        return vec2{
            a * x,
            a * y
        };
    };
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