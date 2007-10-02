/*************************************************************************
 * Copyright (C) 2008 Tavian Barnes <tavianator@gmail.com>               *
 *                                                                       *
 * This file is part of The Carom Library                                *
 *                                                                       *
 * The Carom Library is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as        *
 * published by the Free Software Foundation; either version 3 of the    *
 * License, or (at your option) any later version.                       *
 *                                                                       *
 * The Carom Library is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *************************************************************************/

/*************************************************************************
 * vector/ops.hpp                                                        *
 * ==============                                                        *
 *                                                                       *
 * Arithmetic operators for dealing with vectors are defined here.       *
 *************************************************************************/

#ifndef CAROM_VECTOR_OPS_HPP
#define CAROM_VECTOR_OPS_HPP

#include <mpfr.h>

namespace carom
{
  // Unary operators

  template<int m, int d, int t>
  inline vector_proxy<m, d, t, scalar_unary_proxy<mpfr_t, scalar_pos_op> >
  operator+(const vector_units<m, d, t>& n)
  { return vector_proxy<m, d, t, scalar_unary_proxy<mpfr_t,
                                                    scalar_pos_op> >(n); }

  template<int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, scalar_unary_proxy<op, scalar_pos_op> >
  operator+(const vector_proxy<m, d, t, op>& n)
  { return vector_proxy<m, d, t, scalar_unary_proxy<op, scalar_pos_op> >(n); }

  template<int m, int d, int t>
  inline vector_proxy<m, d, t, scalar_unary_proxy<mpfr_t, scalar_neg_op> >
  operator-(const vector_units<m, d, t>& n)
  { return vector_proxy<m, d, t, scalar_unary_proxy<mpfr_t,
                                                    scalar_neg_op> >(n); }

  template<int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, scalar_unary_proxy<op, scalar_neg_op> >
  operator-(const vector_proxy<m, d, t, op>& n)
  { return vector_proxy<m, d, t, scalar_unary_proxy<op, scalar_neg_op> >(n); }

  // Addition operators

  template<int m, int d, int t>
  inline vector_proxy<m, d, t,
                      scalar_binary_proxy<mpfr_t, mpfr_t, scalar_add_op> >
  operator+(const vector_units<m, d, t>& lhs,
            const vector_units<m, d, t>& rhs)
  { return vector_proxy<m, d, t,
                        scalar_binary_proxy<mpfr_t, mpfr_t,
                                            scalar_add_op> >(lhs, rhs); }

  template<int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, scalar_binary_proxy<mpfr_t, op, scalar_add_op> >
  operator+(const vector_units<m, d, t>& lhs,
            const vector_proxy<m, d, t, op>& rhs)
  { return vector_proxy<m, d, t,
                        scalar_binary_proxy<mpfr_t, op,
                                            scalar_add_op> >(lhs, rhs); }

  template<int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, scalar_binary_proxy<op, mpfr_t, scalar_add_op> >
  operator+(const vector_proxy<m, d, t, op>& lhs,
            const vector_units<m, d, t>& rhs)
  { return vector_proxy<m, d, t,
                        scalar_binary_proxy<op, mpfr_t,
                                            scalar_add_op> >(lhs, rhs); }

  template<int m, int d, int t, typename op1, typename op2>
  inline vector_proxy<m, d, t, scalar_binary_proxy<op1, op2, scalar_add_op> >
  operator+(const vector_proxy<m, d, t, op1>& lhs,
            const vector_proxy<m, d, t, op2>& rhs)
  { return vector_proxy<m, d, t,
                        scalar_binary_proxy<op1, op2,
                                            scalar_add_op> >(lhs, rhs); }

  // Subtraction operators

  template<int m, int d, int t>
  inline vector_proxy<m, d, t,
                      scalar_binary_proxy<mpfr_t, mpfr_t, scalar_sub_op> >
  operator-(const vector_units<m, d, t>& lhs,
            const vector_units<m, d, t>& rhs)
  { return vector_proxy<m, d, t,
                        scalar_binary_proxy<mpfr_t, mpfr_t,
                                            scalar_sub_op> >(lhs, rhs); }

  template<int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, scalar_binary_proxy<mpfr_t, op, scalar_sub_op> >
  operator-(const vector_units<m, d, t>& lhs,
            const vector_proxy<m, d, t, op>& rhs)
  { return vector_proxy<m, d, t,
                        scalar_binary_proxy<mpfr_t, op,
                                            scalar_sub_op> >(lhs, rhs); }

  template<int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, scalar_binary_proxy<op, mpfr_t, scalar_sub_op> >
  operator-(const vector_proxy<m, d, t, op>& lhs,
            const vector_units<m, d, t>& rhs)
  { return vector_proxy<m, d, t,
                        scalar_binary_proxy<op, mpfr_t,
                                            scalar_sub_op> >(lhs, rhs); }

  template<int m, int d, int t, typename op1, typename op2>
  inline vector_proxy<m, d, t, scalar_binary_proxy<op1, op2, scalar_sub_op> >
  operator-(const vector_proxy<m, d, t, op1>& lhs,
            const vector_proxy<m, d, t, op2>& rhs)
  { return vector_proxy<m, d, t,
                        scalar_binary_proxy<op1, op2,
                                            scalar_sub_op> >(lhs, rhs); }

  // Multiplication operators
#if 0
  template<int m1, int m2, int d1, int d2, int t1, int t2>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      scalar_binary_proxy<mpfr_t, mpfr_t, scalar_mul_op> >
  operator*(const scalar_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        scalar_binary_proxy<mpfr_t, mpfr_t,
                                            scalar_mul_op> >(lhs, rhs); }

  template<int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      scalar_binary_proxy<mpfr_t, op, scalar_mul_op> >
  operator*(const scalar_units<m1, d1, t1>& lhs,
            const scalar_proxy<m2, d2, t2, op>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        scalar_binary_proxy<mpfr_t, op,
                                            scalar_mul_op> >(lhs, rhs); }

  template<int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      scalar_binary_proxy<op, mpfr_t, scalar_mul_op> >
  operator*(const scalar_proxy<m1, d1, t1, op>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        scalar_binary_proxy<op, mpfr_t,
                                            scalar_mul_op> >(lhs, rhs); }

  template<int m1, int m2, int d1, int d2, int t1, int t2,
           typename op1, typename op2>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      scalar_binary_proxy<op1, op2, scalar_mul_op> >
  operator*(const scalar_proxy<m1, d1, t1, op1>& lhs,
            const scalar_proxy<m2, d2, t2, op2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        scalar_binary_proxy<op1, op2,
                                            scalar_mul_op> >(lhs, rhs); }

  // Division operators

  template<int m1, int m2, int d1, int d2, int t1, int t2>
  inline scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                      scalar_binary_proxy<mpfr_t, mpfr_t, scalar_div_op> >
  operator/(const scalar_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                        scalar_binary_proxy<mpfr_t, mpfr_t,
                                            scalar_div_op> >(lhs, rhs); }

  template<int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                      scalar_binary_proxy<mpfr_t, op, scalar_div_op> >
  operator/(const scalar_units<m1, d1, t1>& lhs,
            const scalar_proxy<m2, d2, t2, op>& rhs)
  { return scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                        scalar_binary_proxy<mpfr_t, op,
                                            scalar_div_op> >(lhs, rhs); }

  template<int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                      scalar_binary_proxy<op, mpfr_t, scalar_div_op> >
  operator/(const scalar_proxy<m1, d1, t1, op>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                        scalar_binary_proxy<op, mpfr_t,
                                            scalar_div_op> >(lhs, rhs); }

  template<int m1, int m2, int d1, int d2, int t1, int t2,
           typename op1, typename op2>
  inline scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                      scalar_binary_proxy<op1, op2, scalar_div_op> >
  operator/(const scalar_proxy<m1, d1, t1, op1>& lhs,
            const scalar_proxy<m2, d2, t2, op2>& rhs)
  { return scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                        scalar_binary_proxy<op1, op2,
                                            scalar_div_op> >(lhs, rhs); }
#endif // 0
}

#endif // CAROM_VECTOR_OPS_HPP
