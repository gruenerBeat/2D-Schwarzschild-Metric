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
      a.z + z,
    };
  }

  vec3 operator*(double a) {
    return vec3{
      a * x,
      a * y,
      a * z,
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

vec3 Normalize(vec3 a);
//TransformingVector PolarToOrthonormalBasis(TransformingVector v);
double getAngle(double x, double y);
//Basis getPolarBasis(double r, double theta);
vec3 CartesianTransformaion(vec3 coords);
vec3 PolarTransformation(vec3 coords);
//TransformingVector UpdateBasis(TransformingVector v, double t, double r);
//vec3 CalcPolarPositionFormBasis(Basis b);
double cot(double x);

#endif