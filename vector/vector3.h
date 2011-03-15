/*
* Copyright (c) 2010, Mads Andreas Elvheim, mads@mechcore.net
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the organization nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Mads Andreas Elvheim ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Mads Andreas Elvheim BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef VECTOR3_H_GUARD
#define VECTOR3_H_GUARD
#include <cmath>


template<class T>
struct Vector3
{
  T x,y,z;

  Vector3() : x(T(0.0f)), y(T(0.0f)), z(T(0.0f)){}
  Vector3(T a, T b, T c) : x(a), y(b), z(c){}
  
  Vector3<T> operator+(const Vector3<T>& v) const
  {
    return Vector3<T>(x + v.x, y + v.y, z + v.z);
  }
  Vector3<T> operator-(const Vector3<T>& v) const
  {
    return Vector3<T>(x - v.x, y - v.y, z - v.z);
  }
  
  Vector3<T> operator+(const T& v) const
  {
    return Vector3<T>(x+v, y+v, z+v);
  }
  Vector3<T> operator-(const T& v) const
  {
    return Vector3<T>(x-v, y-v, z-v);
  }
  Vector3<T> operator*(const T& v) const
  {
    return Vector3<T>(x*v, y*v, z*v);
  }
  Vector3<T> operator/(const T& v) const
  {
    return Vector3<T>(x/v, y/v, z/v);
  }

  Vector3<T>& operator+=(const Vector3<T>& v)
  {
    *this = *this + v;
    return *this;
  }
  Vector3<T>& operator-=(const Vector3<T>& v)
  {
    *this = *this - v;
    return *this;
  }

  Vector3<T>& operator+=(const T& v)
  {
    *this = *this + v;
    return *this;
  }
  Vector3<T>& operator-=(const T& v)
  {
    *this = *this - v;
    return *this;
  }
  Vector3<T>& operator*=(const T& v)
  {
    *this = *this * v;
    return *this;
  }
  Vector3<T>& operator/=(const T& v)
  {
    *this = *this / v;
    return *this;
  }

  bool operator<(const Vector3<T> v)
  {
	/* Sorry, it's just a hack so we don't have to create a functor to std::sort */
	return z > v.z;
  }

  T length() const
  {
    return std::sqrt(x*x + y*y + z*z);
  }
  Vector3<T> unit() const
  {
    float len = length();
    if(std::abs(len) < 1e-8f)
      return Vector3<T>(0.0f,0.0f,0.0f);
    return *this / len;
  }
  void normalize()
  {
    *this = unit();
  }
};

template<class T>
inline T dot(const Vector3<T>& v1, const Vector3<T>& v2)
{
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<class T>
inline Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2)
{
  return Vector3<T>(v1.y*v2.z - v1.z*v2.y,
		    v1.z*v2.x - v1.x*v2.z,
		    v1.x*v2.y - v1.y*v2.x
		    );
}

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

#endif
