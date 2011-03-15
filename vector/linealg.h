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

#ifndef LINEALG_H_GUARD
#define LINEALG_H_GUARD
#include <cstdlib>
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4.h"

const float PI = 3.1415926535897932384626433832f;

template<class T>
Vector3<T> operator*(const Matrix4<T>& mat, const Vector3<T>& v)
{
  return Vector3<T>(
			v.x*mat[ 0] + v.y*mat[ 1] + v.z*mat[ 2] + mat[ 3],
		    v.x*mat[ 4] + v.y*mat[ 5] + v.z*mat[ 6] + mat[ 7],
		    v.x*mat[ 8] + v.y*mat[ 9] + v.z*mat[10] + mat[11]
		    );
}

template<class T>
Vector4<T> operator*(const Matrix4<T>& mat, const Vector4<T>& v)
{
  return Vector4<T>(
			v.x*mat[ 0] + v.y*mat[ 1] + v.z*mat[ 2] + v.w*mat[ 3],
		    v.x*mat[ 4] + v.y*mat[ 5] + v.z*mat[ 6] + v.w*mat[ 7],
		    v.x*mat[ 8] + v.y*mat[ 9] + v.z*mat[10] + v.w*mat[11],
		    v.x*mat[12] + v.y*mat[13] + v.z*mat[14] + v.w*mat[15]
		    );
}

/* Misc functions */

inline float degtorad(float deg)
{
    return deg/180.0f * PI;
}

inline float radtodeg(float theta)
{
    return theta/PI * 180.0f;
}

template<class T> inline T distance(const Vector3f& v1, const Vector3<T>& v2)
{
    Vector3<T> tmp = v1-v2;
    return tmp.length();
}

template<class T> inline T distance(const Vector4<T>& v1, const Vector4<T>& v2)
{
    Vector4<T> tmp = v1-v2;
    return tmp.length();
}

/* Linear interpolation */

inline Vector2f mix(float t, const Vector2f& v1, const Vector2f& v2)
{
	return v1*(t-1.0f) + v2*t;
}

inline Vector3f mix(float t, const Vector3f& v1, const Vector3f& v2)
{
	return v1*(t-1.0f) + v2*t;
}

inline Vector4f mix(float t, const Vector4f& v1, const Vector4f& v2)
{
	Vector4f v = v1*(t-1.0f) + v2*t;
	v.w = v1.w*(t-1.0f) + v2.w*t;
	return v;
}

/***************************************
 * Transformations for the three axes  *
 ***************************************/

inline Matrix4f rotateX(float deg)
{
    float s = std::sin(degtorad(deg));
    float c = std::cos(degtorad(deg));
    
    return Matrix4f(Vector4f(1.0f, 0.0f, 0.0f, 0.0f),
                    Vector4f(0.0f,    c,   -s, 0.0f),
                    Vector4f(0.0f,    s,    c, 0.0f),
                    Vector4f(0.0f, 0.0f, 0.0f, 1.0f)
                    );
}

inline Matrix4f rotateY(float deg)
{
    float s = std::sin(degtorad(deg));
    float c = std::cos(degtorad(deg));
    
    return Matrix4f(Vector4f(   c, 0.0f,    s, 0.0f),
                    Vector4f(0.0f, 1.0f, 0.0f, 0.0f),
                    Vector4f(  -s, 0.0f,    c, 0.0f),
                    Vector4f(0.0f, 0.0f, 0.0f, 1.0f)
                    );
}

inline Matrix4f rotateZ(float deg)
{
    float s = std::sin(degtorad(deg));
    float c = std::cos(degtorad(deg));
    
    return Matrix4f(Vector4f(   c,   -s, 0.0f, 0.0f),
                    Vector4f(   s,    c, 0.0f, 0.0f),
                    Vector4f(0.0f, 0.0f, 1.0f, 0.0f),
                    Vector4f(0.0f, 0.0f, 0.0f, 1.0f)
                    );
}

inline Matrix4f translate(const Vector4f& offset)
{
    return Matrix4f(
                    Vector4f(1.0f, 0.0f, 0.0f, offset.x),
                    Vector4f(0.0f, 1.0f, 0.0f, offset.y),
                    Vector4f(0.0f, 0.0f, 1.0f, offset.z),
                    Vector4f(0.0f, 0.0f, 0.0f, offset.w)
                    );
}
/***************************************
 * Sets up our perspective clip matrix *
 ***************************************/
inline Matrix4f perspective(float fov, float aspect, float near, float far)
{
    /* Restrict fov to 179 degrees, for numerical stability */
    if(fov >= 180.0f)
        fov = 179.0f;
    
    float y = 1.0f / std::tan(degtorad(fov) * 0.5f);
    float x = y/aspect;
    float z1 = (far+near)/(near-far);
    float z2 = (2.0f*far*near)/(near-far);
    Matrix4f m(Vector4f(x, 0,  0,  0),
               Vector4f(0, y,  0,  0),
               Vector4f(0, 0, z1, z2),
               Vector4f(0, 0,  -1,  0)
               );
    return m;
}

/*********************************************************************
 * Projects our 4D clip coordinates down to 2D viewport coordinates. *
 * z and w are preserved for later use. x and y are now in pixel units*
 *********************************************************************/
inline Vector4f project(const Vector4f& v, float width, float height)
{
    Vector4f proj;
    float centerX = width*0.5f;
    float centerY = height*0.5f;

    proj.x = v.x*centerX  + centerX;
    proj.y = v.y*centerY + centerY;
    proj.z = v.z*0.5f + 0.5f;
    proj.w = 1.0f / v.w;

    return proj;
}

inline float randf(float vmin, float vmax)
{
	return vmin + ((float)rand() / (float)RAND_MAX) * (vmax - vmin);
}

inline Vector3f randv(const Vector3f& vmin, const Vector3f& vmax)
{
	Vector3f p;
	p.x = randf(vmin.x, vmax.x);
	p.y = randf(vmin.y, vmax.y);
	p.z = randf(vmin.z, vmax.z);

	return p;
}

inline Vector3f BezierCurve(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3, const Vector3f& p4, float t)
{
   float mum1,mum13,mu3;
   Vector3f p;

   mum1 = 1.0f - t;
   mum13 = mum1 * mum1 * mum1;
   mu3 = t * t * t;

   p.x = mum13*p1.x + 3.0f*t*mum1*mum1*p2.x + 3.0f*t*t*mum1*p3.x + mu3*p4.x;
   p.y = mum13*p1.y + 3.0f*t*mum1*mum1*p2.y + 3.0f*t*t*mum1*p3.y + mu3*p4.y;
   p.z = mum13*p1.z + 3.0f*t*mum1*mum1*p2.z + 3.0f*t*t*mum1*p3.z + mu3*p4.z;

   return p;
}

inline Vector3f BezierCurve(const std::vector<Vector3f>& p, double t)
{
	int kn,nn,nkn;
	double blend,muk,munk;
	int count = p.size();
	Vector3f b(0.0f, 0.0f, 0.0f);

	muk = 1.0;
	munk = std::pow(1.0-t,(double)count);

	for(unsigned int k=0;k<count;++k) {
		nn = count;
		kn = k;
		nkn = count - k;
		blend = muk * munk;
		muk *= t;
		munk /= (1.0-t);
		while (nn >= 1) {
			blend *= (double)nn;
			--nn;
			if(kn > 1) {
				blend /= (double)kn;
				--kn;
			}
			if (nkn > 1) {
				blend /= (double)nkn;
				--nkn;
			}
		}
		float blendf = static_cast<float>(blend);
		b.x += p[k].x * blendf;
		b.y += p[k].y * blendf;
		b.z += p[k].z * blendf;
	}
	return b;
}

#endif
