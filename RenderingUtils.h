#ifndef RENDERINGUTILS_H
#define RENDERINGUTILS_H

#include "GeometryHelper.h"

struct Color {
    double r;
    double g;
    double b;
    double a;
};

struct Ray {
    bool didHit;
    double hitDst;
    Color hitColor;
    vec8 state;
};

struct Sphere {
    vec3 cartPos;
    double radius;
    Color color;
};

vec3 ToClipPlane(vec3 screenPoint, int screenWidth, int screenHeight, double vfov);

#endif