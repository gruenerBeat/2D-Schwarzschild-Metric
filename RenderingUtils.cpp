#include <stdio.h>
#include <stdlib.h>

#include "RenderingUtils.h"
#include "GeometryHelper.h"

#define _USE_MATH_DEFINES
#include <cmath>

vec3 ToClipPlane(vec3 screenPoint, int screenWidth, int screenHeight, double vfov) {
    double x = (screenPoint.x / screenWidth) * 2.0 - 1.0;
    double y = (screenPoint.y / screenHeight) * 2.0 - 1.0;
    double focal = screenPoint.z;
    double aspect = screenWidth / screenHeight;
    double hfov = 2 * getAngle(aspect * std::tan(vfov / focal), 1);
    return vec3{x * focal * std::tan(hfov / 2), y * focal * std::tan(vfov / 2), focal};
}