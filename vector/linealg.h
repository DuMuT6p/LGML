#ifndef LINEALG_H_GUARD
#define LINEALG_H_GUARD
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4.h"

template<class T>
Vector3<T> operator*(const Matrix4<T>& mat, const Vector3<T>& v)
{
  return Vector4<T>(v.x*mat[ 0] + v.y*mat[ 4] + v.z*mat[ 8] + mat[12],
		    v.x*mat[ 1] + v.y*mat[ 5] + v.z*mat[ 9] + mat[13],
		    v.x*mat[ 2] + v.y*mat[ 6] + v.z*mat[10] + mat[14],
		    v.x*mat[ 3] + v.y*mat[ 7] + v.z*mat[11] + mat[15]
		    );
}

template<class T>
Vector4<T> operator*(const Matrix4<T>& mat, const Vector4<T>& v)
{
  return Vector4<T>(v.x*mat[ 0] + v.y*mat[ 4] + v.z*mat[ 8] + v.w*mat[12],
		    v.x*mat[ 1] + v.y*mat[ 5] + v.z*mat[ 9] + v.w*mat[13],
		    v.x*mat[ 2] + v.y*mat[ 6] + v.z*mat[10] + v.w*mat[14],
		    v.x*mat[ 3] + v.y*mat[ 7] + v.z*mat[11] + v.w*mat[15]
		    );
}

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

typedef Matrix4<int> Matrix4i;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

#endif
