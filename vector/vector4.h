#ifndef VECTOR4_H_GUARD
#define VECTOR4_H_GUARD
#ifdef WIN32
#define _USE_MATH_DEFINES
#else
#define__USE_BSD
#endif
#include <cmath>

#include "vector3.h"

template<class T>
struct Vector4 : public Vector3<T>
{
  T w;
 Vector4() : w(T(1.0)){}
  Vector4(T a, T b, T c, T d = 1.0f) : Vector3<T>(a,b,c), w(d){}
  Vector4(const Vector3<T>& v, T d = 1.0f) : Vector3<T>(v), w(d){}
};

#endif
