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

#ifndef CAROM_SCALAR_OPS_HPP
#define CAROM_SCALAR_OPS_HPP

#include <mpfr.h>
#include <boost/utility.hpp>
#include <cstdlib>
#include <stdexcept>

namespace carom
{
  // Unary operations

  struct scalar_pos_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t n) { mpfr_set(store, n, GMP_RNDN); }
  };

  struct scalar_neg_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t n) { mpfr_neg(store, n, GMP_RNDN); }
  };

  struct scalar_sqrt_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t n) { mpfr_sqrt(store, n, GMP_RNDN); }
  };

  struct scalar_sin_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t n) { mpfr_sin(store, n, GMP_RNDN); }
  };

  struct scalar_cos_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t n) { mpfr_cos(store, n, GMP_RNDN); }
  };

  struct scalar_tan_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t n) { mpfr_tan(store, n, GMP_RNDN); }
  };

  // Binary operations

  struct scalar_add_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t lhs, mpfr_t rhs)
    { mpfr_add(store, lhs, rhs, GMP_RNDN); }
  };

  struct scalar_sub_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t lhs, mpfr_t rhs)
    { mpfr_sub(store, lhs, rhs, GMP_RNDN); }
  };

  struct scalar_mul_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t lhs, mpfr_t rhs)
    { mpfr_mul(store, lhs, rhs, GMP_RNDN); }
  };

  struct scalar_div_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t lhs, mpfr_t rhs) {
      if (mpfr_zero_p(rhs)) { throw std::domain_error("Division by zero"); }
      mpfr_div(store, lhs, rhs, GMP_RNDN);
    }
  };

  // Unary operators

  template <int m, int d, int t>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, scalar_pos_op> >
  operator+(const scalar_units<m, d, t>& n)
  { return scalar_proxy<m, d, t,
                        scalar_unary_proxy<mpfr_t, scalar_pos_op> >(n); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_pos_op> >
  operator+(const scalar_proxy<m, d, t, op>& n)
  { return scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_pos_op> >(n); }

  template <int m, int d, int t>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, scalar_neg_op> >
  operator-(const scalar_units<m, d, t>& n)
  { return scalar_proxy<m, d, t,
                        scalar_unary_proxy<mpfr_t, scalar_neg_op> >(n); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_neg_op> >
  operator-(const scalar_proxy<m, d, t, op>& n)
  { return scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_neg_op> >(n); }

  template <int m, int d, int t>
  inline scalar_proxy<m/2, d/2, t/2, scalar_unary_proxy<mpfr_t, scalar_sqrt_op> >
  sqrt(const scalar_units<m, d, t>& n) {
    // Use TMP to ensure that m, d, and t are even; these typedefs fail to
    // compile if m%2 != 0
    typedef typename boost::enable_if_c<m%2 == 0>::type m_even;
    typedef typename boost::enable_if_c<d%2 == 0>::type d_even;
    typedef typename boost::enable_if_c<t%2 == 0>::type t_even;

    return scalar_proxy<m/2, d/2, t/2,
                        scalar_unary_proxy<mpfr_t, scalar_sqrt_op> >(n);
  }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m/2, d/2, t/2,
		      scalar_unary_proxy<op, scalar_sqrt_op> >
  sqrt(const scalar_proxy<m, d, t, op>& n) {
    // Use TMP to ensure that m, d, and t are even
    typedef typename boost::enable_if_c<m%2 == 0>::type m_even;
    typedef typename boost::enable_if_c<d%2 == 0>::type d_even;
    typedef typename boost::enable_if_c<t%2 == 0>::type t_even;

    return scalar_proxy<m/2, d/2, t/2,
                        scalar_unary_proxy<op, scalar_sqrt_op> >(n);
  }

  template <int m, int d, int t>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, scalar_sin_op> >
  sin(const scalar_units<m, d, t>& n)
  { return scalar_proxy<m, d, t,
                        scalar_unary_proxy<mpfr_t, scalar_sin_op> >(n); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_sin_op> >
  sin(const scalar_proxy<m, d, t, op>& n)
  { return scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_sin_op> >(n); }

  template <int m, int d, int t>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, scalar_cos_op> >
  cos(const scalar_units<m, d, t>& n)
  { return scalar_proxy<m, d, t,
                        scalar_unary_proxy<mpfr_t, scalar_cos_op> >(n); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_cos_op> >
  cos(const scalar_proxy<m, d, t, op>& n)
  { return scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_cos_op> >(n); }

  template <int m, int d, int t>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, scalar_tan_op> >
  tan(const scalar_units<m, d, t>& n)
  { return scalar_proxy<m, d, t,
                        scalar_unary_proxy<mpfr_t, scalar_tan_op> >(n); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_tan_op> >
  tan(const scalar_proxy<m, d, t, op>& n)
  { return scalar_proxy<m, d, t, scalar_unary_proxy<op, scalar_tan_op> >(n); }

  // Addition operators

  template <int m, int d, int t>
  inline scalar_proxy<m, d, t,
                      scalar_binary_proxy<mpfr_t, mpfr_t, scalar_add_op> >
  operator+(const scalar_units<m, d, t>& lhs,
            const scalar_units<m, d, t>& rhs)
  { return scalar_proxy<m, d, t,
                        scalar_binary_proxy<mpfr_t, mpfr_t,
                                            scalar_add_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, op, scalar_add_op> >
  operator+(const scalar_units<m, d, t>& lhs,
            const scalar_proxy<m, d, t, op>& rhs)
  { return scalar_proxy<m, d, t,
                        scalar_binary_proxy<mpfr_t, op,
                                            scalar_add_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_binary_proxy<op, mpfr_t, scalar_add_op> >
  operator+(const scalar_proxy<m, d, t, op>& lhs,
            const scalar_units<m, d, t>& rhs)
  { return scalar_proxy<m, d, t,
                        scalar_binary_proxy<op, mpfr_t,
                                            scalar_add_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op1, typename op2>
  inline scalar_proxy<m, d, t, scalar_binary_proxy<op1, op2, scalar_add_op> >
  operator+(const scalar_proxy<m, d, t, op1>& lhs,
            const scalar_proxy<m, d, t, op2>& rhs)
  { return scalar_proxy<m, d, t,
                        scalar_binary_proxy<op1, op2,
                                            scalar_add_op> >(lhs, rhs); }

  // Subtraction operators

  template <int m, int d, int t>
  inline scalar_proxy<m, d, t,
                      scalar_binary_proxy<mpfr_t, mpfr_t, scalar_sub_op> >
  operator-(const scalar_units<m, d, t>& lhs,
            const scalar_units<m, d, t>& rhs)
  { return scalar_proxy<m, d, t,
                        scalar_binary_proxy<mpfr_t, mpfr_t,
                                            scalar_sub_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, op, scalar_sub_op> >
  operator-(const scalar_units<m, d, t>& lhs,
            const scalar_proxy<m, d, t, op>& rhs)
  { return scalar_proxy<m, d, t,
                        scalar_binary_proxy<mpfr_t, op,
                                            scalar_sub_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, scalar_binary_proxy<op, mpfr_t, scalar_sub_op> >
  operator-(const scalar_proxy<m, d, t, op>& lhs,
            const scalar_units<m, d, t>& rhs)
  { return scalar_proxy<m, d, t,
                        scalar_binary_proxy<op, mpfr_t,
                                            scalar_sub_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op1, typename op2>
  inline scalar_proxy<m, d, t, scalar_binary_proxy<op1, op2, scalar_sub_op> >
  operator-(const scalar_proxy<m, d, t, op1>& lhs,
            const scalar_proxy<m, d, t, op2>& rhs)
  { return scalar_proxy<m, d, t,
                        scalar_binary_proxy<op1, op2,
                                            scalar_sub_op> >(lhs, rhs); }

  // Multiplication operators

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      scalar_binary_proxy<mpfr_t, mpfr_t, scalar_mul_op> >
  operator*(const scalar_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        scalar_binary_proxy<mpfr_t, mpfr_t,
                                            scalar_mul_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      scalar_binary_proxy<mpfr_t, op, scalar_mul_op> >
  operator*(const scalar_units<m1, d1, t1>& lhs,
            const scalar_proxy<m2, d2, t2, op>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        scalar_binary_proxy<mpfr_t, op,
                                            scalar_mul_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      scalar_binary_proxy<op, mpfr_t, scalar_mul_op> >
  operator*(const scalar_proxy<m1, d1, t1, op>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        scalar_binary_proxy<op, mpfr_t,
                                            scalar_mul_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2,
	    typename op1, typename op2>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      scalar_binary_proxy<op1, op2, scalar_mul_op> >
  operator*(const scalar_proxy<m1, d1, t1, op1>& lhs,
            const scalar_proxy<m2, d2, t2, op2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        scalar_binary_proxy<op1, op2,
                                            scalar_mul_op> >(lhs, rhs); }

  // Division operators

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                      scalar_binary_proxy<mpfr_t, mpfr_t, scalar_div_op> >
  operator/(const scalar_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                        scalar_binary_proxy<mpfr_t, mpfr_t,
                                            scalar_div_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                      scalar_binary_proxy<mpfr_t, op, scalar_div_op> >
  operator/(const scalar_units<m1, d1, t1>& lhs,
            const scalar_proxy<m2, d2, t2, op>& rhs)
  { return scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                        scalar_binary_proxy<mpfr_t, op,
                                            scalar_div_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                      scalar_binary_proxy<op, mpfr_t, scalar_div_op> >
  operator/(const scalar_proxy<m1, d1, t1, op>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                        scalar_binary_proxy<op, mpfr_t,
                                            scalar_div_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2,
	    typename op1, typename op2>
  inline scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                      scalar_binary_proxy<op1, op2, scalar_div_op> >
  operator/(const scalar_proxy<m1, d1, t1, op1>& lhs,
            const scalar_proxy<m2, d2, t2, op2>& rhs)
  { return scalar_proxy<m1 - m2, d1 - d2, t1 - t2,
                        scalar_binary_proxy<op1, op2,
                                            scalar_div_op> >(lhs, rhs); }
}

#endif // CAROM_SCALAR_OPS_HPP
