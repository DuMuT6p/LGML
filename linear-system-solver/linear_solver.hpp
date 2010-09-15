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

#ifndef LINEAR_SOLVER_HPP_GUARD
#define LINEAR_SOLVER_HPP_GUARD

#include <vector>
#include <utility>
#include <iostream>
#include "assert.h"
#include "fraction.hpp"

template<class T>
bool SwapRows(int row, std::vector< std::vector<T> >& mat)
{
/* find the first row with mat[i][j] nonzero */
  int i2;
  for(i2=row+1; i2<mat.size(); ++i2){
	if(mat[i2][row] != T(0)){
	  std::swap(mat[row], mat[i2]);
	  break;
	}
  }
  if(i2 == mat.size())
	return false;
  return true;
}

template<class T>
void ScaleRowWithPivot(int row, std::vector< std::vector<T> >& mat)
{
  T scale = mat[row][row];
  if(scale == T(0))
	return;
  for(int j=0; j<mat[row].size(); ++j)
	mat[row][j] /= scale;
}

template<class T>
void BackSubstitution(std::vector< std::vector<T> >& mat)
{
  /* i is the column, j is the row*/
  ASSERT(mat.size() == mat[0].size() - 1);

  for(int i=mat.size()-1; i > 0; --i){
	for(int j=i-1; j >= 0; --j){
	  T scale = mat[j][i];
	  for(int k=0; k<mat[j].size(); ++k){
		mat[j][k] -= mat[i][k] * scale;
	  }
	}
  }
}

template<class T>
bool linear_solver(std::vector< std::vector<T> >& mat, std::vector<T>& vec)
{
  if(mat.size() != vec.size())
	return false;

  /* Combine the matrix and the vector */
  for(int i=0; i<mat.size(); ++i)
	mat[i].push_back(vec[i]);

  /********************************************
   ** First, get matrix into triangular form **
   ********************************************/

  /* for each row, reduce unknowns for each row in order*/
  for(int i=0; i<mat.size(); ++i){
	/* If the pivot is zero */
	if(mat[i][i] == T(0)){
	  /* find the first row with mat[i][j] nonzero */
	  if(SwapRows(i, mat) == false)
		return false;
	}
	
	/* Scale the row with the pivot so the pivot becomes 1.0*/
	ScaleRowWithPivot(i, mat);
	
	/* eliminate the unknown mat[i][i] for all rows under column i.
	   This is done by scaling row i with the pivot at row i2, and adding
	   that to row i2. */
	
	for(int i2=i+1; i2<mat.size(); ++i2){
	  /* Skip rows that are already reduced */
	  if(mat[i2][i] == T(0))
		continue;

	  T scale = T(mat[i2][i]) / T(mat[i][i]);
	  
	  /* Make sure our rows are of the same length */
	  if(mat[i2].size() != mat[i].size())
		return false;

	  for(int j=0; j<mat[i2].size(); ++j){
		mat[i2][j] -= (mat[i][j] * scale);
	  }
	}
  }

  /***********************************************************************
   ** Matrix is now in triangular form, now we can do back-substitution **
   ***********************************************************************/
  BackSubstitution(mat);

  for(int i=0; i<vec.size(); ++i)
	vec[i] = mat[i][mat.size()];

  return true;
}

template<class T>
void PrintVector(const std::vector<T>& vec)
{
  for(int i=0; i<vec.size(); ++i){
	  std::cout << vec[i] << " ";
  }
  std::cout << std::endl;
}

template<class T>
void PrintMatrix(const std::vector< std::vector<T> >& mat)
{
  for(int i=0; i<mat.size(); ++i){
	PrintVector(mat[i]);
  }
  std::cout << std::endl;
}

#endif

/*
  (x*0.1+y*0.2) / (x + y) < 0.5
  x*0.1 + y*0.2 < x*0.5 + y*0.5
  x*0.1 + y*0.2 - x*0.5 - y*0.5 < 0

  -x*0.4 - y*0.3 < 0 OR
   x*0.4 + y*0.3 > 0
*/
