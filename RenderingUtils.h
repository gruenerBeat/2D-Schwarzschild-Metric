#ifndef RENDERINGUTILS_H
#define RENDERINGUTILS_H

#include "GeometryHelpler.h"

struct Color {
    double r;
    double g;
    double b;
    double a;
}

struct HitInfo {
    bool didHit;
    double hitDst;
    Color hitColor;
    vec8 state;
};

vec3 ToClipPlane(vec3 screenPoint, int screenWidth, int screenWHeight, double vfov);

#endif