#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

struct vec2
{
    double x;
    double y;

    vec2
    operator+(const vec2 a)
    {
	return vec2{a.x + x, a.y + y};
    }

    vec2
    operator-(vec2 a)
    {
	return vec2{-a.x + x, -a.y + y};
    }
};

struct vec8
{
    double y1;
    double y2;
    double y3;
    double y4;
    double y5;
    double y6;
    double y7;
    double y8;

    vec8
    operator+(vec8 a)
    {
	return vec8{a.y1 + y1, a.y2 + y2, a.y3 + y3, a.y4 + y4, a.y5 + y5, a.y6 + y6, a.y7 + y7, a.y8 + y8};
    }

    vec8
    operator*(double a)
    {
	return vec8{a * y1, a * y2, a * y3, a * y4, a * y5, a * y6, a * y7, a * y8};
    }
};

struct Basis
{
    vec2 e1;
    vec2 e2;
};

struct TransformingVector
{
    Basis basis;
    vec2 components;
};

TransformingVector
PolarToOrthonormalBasis(TransformingVector v);
double
getAngle(double x, double y);
Basis
getPolarBasis(double r, double theta);
vec2
CartesianTransformaion(double r, double t);
vec2
PolarTransformation(double x, double y);
TransformingVector
UpdateBasis(TransformingVector v, double t, double r);
vec2
CalcPolarPositionFormBasis(Basis b);
double
cot(double x);

#endif
