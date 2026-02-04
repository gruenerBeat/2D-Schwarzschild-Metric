#include "Screen.h"

vec2 ToScreenCoords(vec2 coords, int width, int height, int size) {
    return vec2{
        ((coords.x * size + 1) / 2) * width,
        ((coords.y * size + 1) / 2) * height
    };
}