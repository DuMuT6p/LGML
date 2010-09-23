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

#ifndef FRACTION_H_GUARD
#define FRACTION_H_GUARD
//#include <boost/static_assert.h>

/* Greatest common divisor */
inline int gcd(int a, int b)
{
  int c;
  int sign1,sign2;
  
  if(a < 0){
    sign1 = -1;
    a = -a;
  } else {
    sign1 = 1;
  }
  
  if(b < 0){
    sign2 = -1;
    b = -b;
  } else {
    sign2 = 1;
  }
  
  while(b){
    c = b;
    b = a%b;
    a = c;
  }
  return a*sign1*sign2;
}
/* Least common multiple */
inline int lcm(int a, int b)
{
  return (a / gcd(a,b)) * b;
}

template<class T> class Fraction
{
  T num, denom;
public:
  Fraction() : num(0), denom(1){}
  Fraction(T numerator, T denominator=T(1)) : num(numerator), denom(denominator)
  {
    Simplify();
  }

  T Numerator() const { return num; }
  T Denominator() const { return denom; }
  float DividedValue() const { return (float)num / (float)denom; }

  bool IsFraction() const
  {
    if(denom > 1)
      return true;
    return false;
  }

  Fraction<T> operator+(const Fraction<T>& right) const
  {
	
    T n1 = num;
    T n2 = right.num;
    T d1 = denom;
    T d2 = right.denom;

    T d3 = lcm(d1, d2);
    T ma = d3 / d1;
    T mb = d3 / d2;

    return Fraction<T>(n1*ma + n2*mb, d3);
  }
  Fraction<T> operator-(const Fraction<T>& right) const
  {
    T n1 = num;
    T n2 = right.num;
    T d1 = denom;
    T d2 = right.denom;

    T d3 = lcm(d1, d2);
    T ma = d3 / d1;
    T mb = d3 / d2;

    return Fraction<T>(n1*ma - n2*mb, d3);
  }
  Fraction<T> operator*(const Fraction<T>& right) const
  {
    T n1 = num;
    T n2 = right.num;
    T d1 = denom;
    T d2 = right.denom;

    return Fraction<T>(n1*n2,
		       d1*d2);
  }
  Fraction<T> operator/(const Fraction<T>& right) const
  {
    /* swap here */  
    Fraction<T> tmp(right.Denominator(), right.Numerator());

    T n1 = num;
    T d1 = denom;
    T n2 = tmp.Numerator();
    T d2 = tmp.Denominator();
	
    return Fraction<T>(n1*n2,
		       d1*d2);
  }

  Fraction<T>& operator+=(const Fraction<T>& right)
  {
    *this = *this + right;
    Simplify();
    return *this;
  }
  Fraction<T>& operator-=(const Fraction<T>& right)
  {
    *this = *this - right;
    Simplify();
    return *this;
  }
  Fraction<T>& operator*=(const Fraction<T>& right)
  {
    *this = *this * right;
    Simplify();
    return *this;
  }
  Fraction<T>& operator/=(const Fraction<T>& right)
  {
    *this = *this / right;
    Simplify();
    return *this;
  }

  bool operator==(const Fraction<T>& right) const
  {
    if((num == right.num) && (denom == right.denom))
      return true;
    return false;
  }
  
  bool operator!=(const Fraction<T>& right) const
  {
    if((num != right.num) || (denom != right.denom))
      return true;
    return false;
  }
  
private:
  void Simplify()
  {
    /* make sure that *zero* is represented as (0/1) */
    if(denom == T(0)){
      num = T(0);
      denom = T(1);
    }
    else if(num == T(0)){
      denom = T(1);
    }
	
    /* make sure negative denominators are fixed.
       (only numerators can be negative) */
    if(denom < T(0)){
      num = -num;
      denom = -denom;
    }

    /* make sure divi never becomes negative */
    int sign;
    if(num < 0){
      num = -num;
      sign = -1;
    } else
      sign = 1;
	
    /* convert to lowest terms by dividing with the gcd */
    T divi = gcd(num, denom);
    if(divi > 1){
      num /= divi;
      denom /= divi;
    }
    /* restore the initial sign */
    num *= sign;
	
  }
};

#endif
