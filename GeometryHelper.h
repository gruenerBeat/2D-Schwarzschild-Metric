#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

struct vec3 {
  double x;
  double y;
  double z;

  vec3 operator+(vec3 a) {
    return vec3{
      a.x + x,
      a.y + y,
      a.z + z
    };
  }

  vec3 operator*(double a) {
    return vec3{
      a * x,
      a * y,
      a * z
    };
  }

  vec3 operator-(vec3 a) {
    return vec3{
      x - a.x,
      y - a.y,
      z - a.z
    };
  }
};

struct vec4 {
  double x;
  double y;
  double z;
  double w;

  vec4 operator+(vec4 a) {
    return vec4{
      a.x + x,
      a.y + y,
      a.z + z,
      a.w + w
    };
  }

  vec4 operator*(double a) {
    return vec4{
      a * x,
      a * y,
      a * z,
      a * w
    };
  }
};

struct vec8 {
    double y1;
    double y2;
    double y3;
    double y4;
    double y5;
    double y6;
    double y7;
    double y8;

    vec8 operator+(vec8 a) {
      return vec8{
        a.y1 + y1,
        a.y2 + y2,
        a.y3 + y3,
        a.y4 + y4,
        a.y5 + y5,
        a.y6 + y6,
        a.y7 + y7,
        a.y8 + y8
      };
    }

    vec8 operator*(double a) {
      return vec8{
        a * y1,
        a * y2,
        a * y3,
        a * y4,
        a * y5,
        a * y6,
        a * y7,
        a * y8
      };
    }
};

struct Matrix3x3 {
  vec3 Col1;
  vec3 Col2;
  vec3 Col3;

  vec3 operator*(vec3 a) {
    return vec3{
      Col1.x * a.x + Col2.x * a.y + Col3.x * a.z,
      Col1.y * a.x + Col2.y * a.y + Col3.y * a.z,
      Col1.z * a.x + Col2.z * a.y + Col3.z * a.z
    };
  }
};

vec3 Normalize(vec3 a);
double getAngle(double x, double y);
vec3 CartesianTransformaion(vec3 coords);
vec3 PolarTransformation(vec3 coords);
vec3 Cross(vec3 a, vec3 b);
double Dot(vec3 a, vec3 b);
vec4 ToVec4(vec3 a, double b);
vec3 ToVec3(vec4 a);
double cot(double x);
vec3 PolarTransformationAt(vec3 polarCoords, vec3 vector);

#endif