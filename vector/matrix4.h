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

#ifndef MATRIX4_H_GUARD
#define MATRIX4_H_GUARD
#include <algorithm>
#include "vector4.h"

template<class T> struct Matrix4
{
  T m[16];
  Matrix4(){ zero(); }
  Matrix4(const Vector4<T>& c1,
	  const Vector4<T>& c2,
	  const Vector4<T>& c3,
	  const Vector4<T>& c4)
  {
    m[ 0] = c1.x; m[ 1] = c1.y; m[ 2] = c1.z; m[ 3] = c1.w;
    m[ 4] = c2.x; m[ 5] = c2.y; m[ 6] = c2.z; m[ 7] = c2.w;
    m[ 8] = c3.x; m[ 9] = c3.y; m[10] = c3.z; m[11] = c3.w;
    m[12] = c4.x; m[13] = c4.y; m[14] = c4.z; m[15] = c4.w;
  }

  void zero()
  {
	std::fill(m, m+16, T(0.0f));
  }
  void identity()
  {
    std::fill(m, m+16, T(0.0f));
    m[0] = m[5] = m[10] = m[15] = 1.0f;
  }
  T operator[](size_t index) const
  {
    return m[index];
  }

  T& operator[](size_t index)
  {
      return m[index];
  }

  const T* c_ptr() const { return m; }

  Matrix4<T> operator*(const Matrix4<T>& mat) const
  {
    Matrix4<T> result;
    for(int i=0; i<4; ++i){
      for(int j=0; j<4; ++j){
		for(int k=0; k<4; ++k){
			result[j + i*4] += m[k + i*4]*mat[j + k*4];
		}
      }
    }
    return result;
  }
};

typedef Matrix4<int> Matrix4i;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

#endif
