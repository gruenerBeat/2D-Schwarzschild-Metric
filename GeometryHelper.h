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
  }

  vec2 operator*(double a) {
    return vec2{
      a * x,
      a * y
    };
  }

  vec2 operator-(vec2 a) {
    return vec2{
      x - a.x,
      y - a.y
    };
  }
};

struct Basis {
  vec2 e1;
  vec2 e2;
};

Basis getBasis(vec2 position);

#endif