/*
* Copyright (c) 2009, Mads Andreas Elvheim, mads@mechcore.net
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

#ifndef FIXEDPOINT_HPP_GUARD
#define FIXEDPOINT_HPP_GUARD

#include <limits>
#include <inttypes.h>
#include <boost/static_assert.hpp>



/* Class used to find the "next" greatest (range) type.
 * Used by Math::Fixed to choose a bigger type used for casting, to avoid overflows
 */
	
template<class _Ty> struct Type_Next_Greater_Than
{
  struct ERROR_NO_VALID_CAST{};
  typedef ERROR_NO_VALID_CAST value_type;
private:
  Type_Next_Greater_Than();
  ~Type_Next_Greater_Than();
};

template<> struct Type_Next_Greater_Than <bool>
{	
  typedef int8_t value_type;
};

template<> struct Type_Next_Greater_Than <int8_t>
{	
  typedef int16_t value_type;
};

template<> struct Type_Next_Greater_Than <int16_t>
{	
  typedef int32_t value_type;
};

template<> struct Type_Next_Greater_Than <int32_t>
{	
  typedef int64_t value_type;
};

/* Return the type with the greatest range*/
	
template<class _Ty, class _Other> struct Type_Max
{
  struct ERROR_NO_VALID_CAST{};
  typedef ERROR_NO_VALID_CAST value_type;
private:
  Type_Max();
};

template<> struct Type_Max <int8_t, int8_t>
{
  typedef int8_t value_type;
};

template<> struct Type_Max <int8_t, int16_t>
{
  typedef int16_t value_type;
};

template<> struct Type_Max <int8_t, int32_t>
{
  typedef int32_t value_type;
};

template<> struct Type_Max <int16_t, int8_t>
{
  typedef int16_t value_type;
};

template<> struct Type_Max <int16_t, int16_t>
{
  typedef int16_t value_type;
};

template<> struct Type_Max <int16_t, int32_t>
{
  typedef int32_t value_type;
};

template<> struct Type_Max <int32_t, int8_t>
{
  typedef int32_t value_type;
};

template<> struct Type_Max <int32_t, int16_t>
{
  typedef int32_t value_type;
};

template<> struct Type_Max <int32_t, int32_t>
{
  typedef int32_t value_type;
};
	
template<unsigned _Pos, unsigned _Other_Pos> struct Pos_Min
{
  static const unsigned value = _Pos < _Other_Pos ? _Pos : _Other_Pos;
};

template<unsigned _Pos, unsigned _Other_Pos> struct Pos_Max
{
  static const unsigned value = _Pos > _Other_Pos ? _Pos : _Other_Pos;
};

template<unsigned _Pos, unsigned _Other_Pos> struct Pos_Min_Bool
{
  static const bool value = _Pos < _Other_Pos ? true : false;
};

template<unsigned _Pos, unsigned _Other_Pos> struct Pos_Max_Bool
{
  static const bool value = _Pos > _Other_Pos ? true : false;
};

/* Helper class for doing arithmetic shift left or right depending on a compile-time constant */
template<bool b> struct EvaluateShift{};
template<> struct EvaluateShift <true>
{
  template<class _Ty1, class _Ty2> static _Ty1 Shift(const _Ty2& _Left, const _Ty2& _Right)
  {
    return _Left << _Right;
  }
};

template<> struct EvaluateShift <false>
{
  template<class _Ty1, class _Ty2> static _Ty1 Shift(const _Ty2& _Left, const _Ty2& _Right)
  {
    return _Left >> _Right;
  }
};

/* Fixedpoint class */

template<class _Ty = int32_t, unsigned _Pos = 16> class Fixed
{
public:
  typedef _Ty value_type;
  _Ty _val;

  /* Fixedpoint / bits mismatch */
  BOOST_STATIC_ASSERT(static_cast<unsigned>(std::numeric_limits<_Ty>::digits) > _Pos);
  BOOST_STATIC_ASSERT(std::numeric_limits<_Ty>::is_integer);
  BOOST_STATIC_ASSERT(std::numeric_limits<_Ty>::is_signed);

public:
  Fixed() : _val(0){}
		
  explicit Fixed(char value) : _val(static_cast<_Ty>(value << _Pos )){}
  explicit Fixed(short value) : _val(static_cast<_Ty>(value << _Pos )){}
  explicit Fixed(int value) : _val(static_cast<_Ty>(value << _Pos )){}
  explicit Fixed(long value) : _val(static_cast<_Ty>(value << _Pos )){}
  explicit Fixed(float value) : _val(static_cast<_Ty>(value * static_cast<float>( 1 << _Pos ))){}

  template<class _Other, unsigned _Pos_Other> Fixed(const Fixed<_Other, _Pos_Other>& _Right)
  {
    _val = _Convert(_Right);
  }

  Fixed(const Fixed<_Ty, _Pos>& _Right)
  {
    _val = _Right._val;
  }

  template<class _Other, unsigned _Pos_Other>
  Fixed<_Ty, _Pos> operator+ (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return _FromInteger(left._val + right._val);
  }

  template<class _Other, unsigned _Pos_Other>
  Fixed<_Ty, _Pos> operator- (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return _FromInteger(left._val - right._val);
  }

  template<class _Other, unsigned _Pos_Other>
  Fixed<_Ty, _Pos> operator* (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return _FromInteger( (static_cast<typename Type_Next_Greater_Than<typename Type_Max<_Ty, _Other>::value_type >::value_type >(left._val) * right._val) >> Pos_Max<_Pos, _Pos_Other>::value );
  }

  template<class _Other, unsigned _Pos_Other>
  Fixed<_Ty, _Pos> operator/ (const Fixed<_Other, _Pos_Other>& _Right) const
  { 
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    //typename Type_Next_Greater_Than<typename Type_Max<_Ty, _Other>::value_type >::value_type castup = left._val;
    //castup <<= Pos_Max<_Pos, _Pos_Other>::value; //fix
    //castup /= right._val;
    //return _FromInteger(static_cast<_Ty>(castup));
    return _FromInteger( (static_cast<typename Type_Next_Greater_Than<typename Type_Max<_Ty, _Other>::value_type >::value_type >(left._val) << Pos_Max<_Pos, _Pos_Other>::value) / right._val );
  }
		
  template<class _Other, unsigned _Pos_Other>
  Fixed<_Ty, _Pos>& operator+= (const Fixed<_Other, _Pos_Other>& _Right)
  {
    *this = (*this) + _Right;
    return (*this);
  }

  template<class _Other, unsigned _Pos_Other>
  Fixed<_Ty, _Pos>& operator-= (const Fixed<_Other, _Pos_Other>& _Right)
  {
    *this = (*this) - _Right;
    return (*this);
  }

  template<class _Other, unsigned _Pos_Other>
  Fixed<_Ty, _Pos>& operator*= (const Fixed<_Other, _Pos_Other>& _Right)
  {
    *this = (*this) * _Right;
    return (*this);
  }

  template<class _Other, unsigned _Pos_Other>
  Fixed<_Ty, _Pos>& operator/= (const Fixed<_Other, _Pos_Other>& _Right)
  {
    *this = (*this) / _Right;
    return (*this);
  }
		
  template<class _Other, unsigned _Pos_Other>
  bool operator< (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return left._val < right._val;
  }
		
  template<class _Other, unsigned _Pos_Other>
  bool operator> (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return left._val > right._val;
  }
		
  template<class _Other, unsigned _Pos_Other>
  bool operator<= (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return left._val <= right._val;
  }
		
  template<class _Other, unsigned _Pos_Other>
  bool operator>= (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return left._val >= right._val;
  }
		
  template<class _Other, unsigned _Pos_Other>
  bool operator== (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return left._val == right._val;
  }
		
  template<class _Other, unsigned _Pos_Other>
  bool operator!= (const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > left = *this;
    const Fixed< typename Type_Max<_Ty, _Other>::value_type, Pos_Max<_Pos, _Pos_Other>::value > right = _Right;
    return left._val != right._val;
  }
		

  bool operator!(void) const
  {
    return !_val;
  }
		
  Fixed<_Ty, _Pos> operator++(void)
  {
    *this = *this + Fixed<_Ty, _Pos>(1);
    return *this;
  }
		
  Fixed<_Ty, _Pos> operator++(int)
  {
    Fixed<_Ty, _Pos> tmp(*this);
    *this = *this + Fixed<_Ty, _Pos>(1);
    return tmp;
  }
		
  Fixed<_Ty, _Pos> operator--(void)
  {
    *this = *this - Fixed<_Ty, _Pos>(1);
    return *this;
  }
		
  Fixed<_Ty, _Pos> operator--(int)
  {
    Fixed<_Ty, _Pos> tmp(*this);
    *this = *this - Fixed<_Ty, _Pos>(1);
    return tmp;
  }
		
  Fixed<_Ty, _Pos> floor(void) const
  {
    Fixed<_Ty, _Pos> tmpf;
    tmpf._val &= ~((1<<_Pos) - 1);
    return tmpf;
  }
		
  Fixed<_Ty, _Pos> ceil(void) const
  {
    Fixed<_Ty, _Pos> tmpf;
    tmpf._val += ((static_cast<_Ty>(1)<<_Pos) - static_cast<_Ty>(1));
    tmpf._val &= ~((static_cast<_Ty>(1)<<_Pos) - static_cast<_Ty>(1));
    return tmpf;
  }

  const value_type& GetFixed() const { return _val; }
  value_type GetValue() const { return _val >> _Pos; } /* normal integer */
  float GetFloatFixed() const { return static_cast<float>(_val); }
  float GetFloatValue() const { return static_cast<float>(_val >> _Pos); } /* normal integer */
  unsigned GetPoint() const { return _Pos; } /* Point placement between fraction and real */
private:
  template<class _Other, unsigned _Pos_Other> _Ty _Convert(const Fixed<_Other, _Pos_Other>& _Right) const
  {
    const unsigned posdiff = Pos_Max<_Pos, _Pos_Other>::value - Pos_Min<_Pos, _Pos_Other>::value;
    return EvaluateShift< Pos_Max_Bool<_Pos, _Pos_Other>::value>::template Shift< typename Type_Max<_Ty, _Other>::value_type, typename Type_Max<_Ty, _Other>::value_type>(_Right.GetFixed(), posdiff);
  }

  template<class Type2> Fixed<_Ty, _Pos> _FromInteger(Type2 value) const
  {
    Fixed<_Ty, _Pos> tmpf;
    tmpf._val = value;
    return tmpf;
  }
};

template<class _Ty, unsigned _Pos> static Fixed<_Ty, _Pos> floor(const Fixed<_Ty, _Pos>& val)
{
  return val.floor();
}

template<class _Ty, unsigned _Pos> static Fixed<_Ty, _Pos> ceil(const Fixed<_Ty, _Pos>& val)
{
  return val.ceil();
}

/* ********************************************************
   Temporary use floats here until we find a better solution *
********************************************************/
template<class _Ty, unsigned _Pos> static Fixed<_Ty, _Pos> cos(const Fixed<_Ty, _Pos>& val)
{
  float f = val.GetFloatValue();
  float c = cos(f);
  return Fixed<_Ty, _Pos>(c);
}

template<class _Ty, unsigned _Pos> static Fixed<_Ty, _Pos> sin(const Fixed<_Ty, _Pos>& val)
{
  float f = val.GetFloatValue();
  float s = sin(f);
  return Fixed<_Ty, _Pos>(s);
}

template<class _Ty, unsigned _Pos> static Fixed<_Ty, _Pos> tan(const Fixed<_Ty, _Pos>& val)
{
  float f = val.GetFloatValue();
  float t = tan(f);
  return Fixed<_Ty, _Pos>(t);
}

template<class _Ty, unsigned _Pos> static Fixed<_Ty, _Pos> sqrt(const Fixed<_Ty, _Pos>& val)
{
  float f = val.GetFloatValue();
  float sq = sqrt(f);
  return Fixed<_Ty, _Pos>(sq);
}

#endif

