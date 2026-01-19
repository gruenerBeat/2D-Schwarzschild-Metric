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

  //Operate on Vector
  vec3 operator*(vec3 a) {
    return vec3{
      Col1.x * a.x + Col2.x * a.y + Col3.x * a.z,
      Col1.y * a.x + Col2.y * a.y + Col3.y * a.z,
      Col1.z * a.x + Col2.z * a.y + Col3.z * a.z
    };
  }

  //Scaling
  Matrix3x3 operator*(double a) {
    return Matrix3x3{
      vec3{a * Col1.x, a * Col2.x, a * Col3.x},
      vec3{a * Col1.y, a * Col2.y, a * Col3.y},
      vec3{a * Col1.z, a * Col2.z, a * Col3.z}
    };
  }

  //Transpose
  Matrix3x3 operator~() {
    return Matrix3x3 {
      vec3{Col1.x, Col1.y, Col1.z},
      vec3{Col2.x, Col2.y, Col2.z},
      vec3{Col3.x, Col3.y, Col3.z}
    };
  }

  //Inverse
  Matrix3x3 operator!() {
    double determinant = Col1.x * (Col2.y * Col3.z - Col3.y * Col2.z) - Col2.x * (Col1.y * Col3.z - Col3.y * Col1.z) + Col3.x * (Col1.y * Col2.z - Col2.y * Col1.z);
    Matrix3x3 cofactors = Matrix3x3{
      vec3{(Col2.y * Col3.z - Col2.z * Col3.y), -(Col2.x * Col3.z - Col2.z * Col3.x), (Col2.x * Col3.y - Col2.y * Col3.x)},
      vec3{-(Col1.y * Col3.z - Col1.z * Col3.y), (Col1.x * Col3.z - Col1.z * Col3.x), -(Col1.x * Col3.y - Col1.y * Col3.x)},
      vec3{(Col1.y * Col2.z - Col1.z * Col2.y), -(Col1.x * Col2.z - Col1.z * Col2.x), (Col1.x * Col2.y - Col1.y * Col2.x)}
    };
    Matrix3x3 adjoint = ~cofactors;
    return adjoint * (1 / determinant);
  }
};



struct Matrix4x4 {
  vec4 Row1;
  vec4 Row2;
  vec4 Row3;
  vec4 Row4;

  vec4 operator*(vec4 a) {
    return vec4{
      Row1.x * a.x + Row1.y * a.y + Row1.z * a.z + Row1.w * a.w,
      Row2.x * a.x + Row2.y * a.y + Row2.z * a.z + Row2.w * a.w,
      Row3.x * a.x + Row3.y * a.y + Row3.z * a.z + Row3.w * a.w,
      Row4.x * a.x + Row4.y * a.y + Row4.z * a.z + Row4.w * a.w
    };
  }

  Matrix4x4 operator*(double a) {
    return Matrix4x4{
      Row1 * a,
      Row2 * a,
      Row3 * a,
      Row4 * a
    };
  }
};

vec3 Normalize(vec3 a);
double getAngle(double x, double y);
vec3 CartesianTransformaion(vec3 coords);
vec3 PolarTransformation(vec3 coords);
vec3 Cross(vec3 a, vec3 b);
double Dot(vec3 a, vec3 b);
double Dot4(vec4 a, vec4 b);
vec4 ToVec4(vec3 a, double b);
vec3 ToVec3(vec4 a);
double cot(double x);
vec3 SphericalTransformationAt(vec3 polarCoords, vec3 vector);
Matrix4x4 Inverse(Matrix4x4 a);

#endif