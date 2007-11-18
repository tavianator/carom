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

#ifndef CAROM_VECTOR_OPS_HPP
#define CAROM_VECTOR_OPS_HPP

#include <mpfr.h>

namespace carom
{
  // Unary operations

  struct vector_pos_op
  {
  public:
    static void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z,
                     mpfr_t x,       mpfr_t y,       mpfr_t z) {
      mpfr_set(store_x, x, GMP_RNDN);
      mpfr_set(store_y, y, GMP_RNDN);
      mpfr_set(store_z, z, GMP_RNDN);
    }
  };

  struct vector_neg_op
  {
  public:
    static void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z,
                     mpfr_t x,       mpfr_t y,       mpfr_t z) {
      mpfr_neg(store_x, x, GMP_RNDN);
      mpfr_neg(store_y, y, GMP_RNDN);
      mpfr_neg(store_z, z, GMP_RNDN);
    }
  };

  struct scalar_norm_op
  {
  public:
    static void eval(mpfr_t store, mpfr_t x, mpfr_t y, mpfr_t z) {
      // store = sqrt(x*x + y*y + z*z);
      mpfr_t temp;
      mpfr_init(temp);
      mpfr_sqr(temp, x, GMP_RNDN);           // temp   = x*x;
      mpfr_sqr(store, y, GMP_RNDN);          // store  = y*y;
      mpfr_add(temp, temp, store, GMP_RNDN); // temp  += store;
      mpfr_sqr(store, z, GMP_RNDN);          // store  = z*z;
      mpfr_add(temp, temp, store, GMP_RNDN); // temp  += store;
      mpfr_sqrt(store, temp, GMP_RNDN);      // store  = sqrt(temp);
      mpfr_clear(temp);
    }
  };

  struct vector_normalized_op
  {
  public:
    static void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z,
                     mpfr_t x,       mpfr_t y,       mpfr_t z) {
      // store = v / sqrt(x*x + y*y + z*z);
      mpfr_t temp1, temp2;
      mpfr_init(temp1);
      mpfr_init(temp2);
      mpfr_sqr(temp2, x, GMP_RNDN);            // temp2  = x*x;
      mpfr_sqr(temp1, y, GMP_RNDN);            // temp1  = y*y;
      mpfr_add(temp2, temp2, temp1, GMP_RNDN); // temp2 += temp1;
      mpfr_sqr(temp1, z, GMP_RNDN);            // temp1  = z*z;
      mpfr_add(temp2, temp2, temp1, GMP_RNDN); // temp2 += temp1;
      mpfr_sqrt(temp1, temp2, GMP_RNDN);       // temp1  = sqrt(temp2);
      mpfr_div(store_x, x, temp1, GMP_RNDN);   // store_x  = x / temp1;
      mpfr_div(store_y, y, temp1, GMP_RNDN);   // store_y  = y / temp1;
      mpfr_div(store_z, z, temp1, GMP_RNDN);   // store_z  = z / temp1;
      mpfr_clear(temp1);
      mpfr_clear(temp2);
    }
  };

  // Binary operations

  struct vector_add_op
  {
  public:
    static void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z,
                     mpfr_t lhs_x,   mpfr_t lhs_y,   mpfr_t lhs_z,
                     mpfr_t rhs_x,   mpfr_t rhs_y,   mpfr_t rhs_z) {
      mpfr_add(store_x, lhs_x, rhs_x, GMP_RNDN);
      mpfr_add(store_y, lhs_y, rhs_y, GMP_RNDN);
      mpfr_add(store_z, lhs_z, rhs_z, GMP_RNDN);
    }
  };

  struct vector_sub_op
  {
  public:
    static void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z,
                     mpfr_t lhs_x,   mpfr_t lhs_y,   mpfr_t lhs_z,
                     mpfr_t rhs_x,   mpfr_t rhs_y,   mpfr_t rhs_z) {
      mpfr_sub(store_x, lhs_x, rhs_x, GMP_RNDN);
      mpfr_sub(store_y, lhs_y, rhs_y, GMP_RNDN);
      mpfr_sub(store_z, lhs_z, rhs_z, GMP_RNDN);
    }
  };

  struct vector_mul_op
  {
  public:
    static void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z,
                     mpfr_t lhs_x,   mpfr_t lhs_y,   mpfr_t lhs_z,
                     mpfr_t rhs) {
      mpfr_mul(store_x, lhs_x, rhs, GMP_RNDN);
      mpfr_mul(store_y, lhs_y, rhs, GMP_RNDN);
      mpfr_mul(store_z, lhs_z, rhs, GMP_RNDN);
    }
  };

  struct vector_div_op
  {
  public:
    static void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z,
                     mpfr_t lhs_x,   mpfr_t lhs_y,   mpfr_t lhs_z,
                     mpfr_t rhs) {
      mpfr_div(store_x, lhs_x, rhs, GMP_RNDN);
      mpfr_div(store_y, lhs_y, rhs, GMP_RNDN);
      mpfr_div(store_z, lhs_z, rhs, GMP_RNDN);
    }
  };

  struct scalar_dot_op
  {
  public:
    static void eval(mpfr_t store,
                     mpfr_t lhs_x,   mpfr_t lhs_y,   mpfr_t lhs_z,
                     mpfr_t rhs_x,   mpfr_t rhs_y,   mpfr_t rhs_z) {
      // store = lhs_x*rhs_x + lhs_y*rhs_y + lhs_z*rhs_z;
      mpfr_t temp;
      mpfr_init(temp);
      mpfr_mul(store, lhs_x, rhs_x, GMP_RNDN); // store  = x*x;
      mpfr_mul(temp, lhs_y, rhs_y, GMP_RNDN);  // temp   = y*y;
      mpfr_add(store, store, temp, GMP_RNDN);  // store += temp;
      mpfr_mul(temp, lhs_z, rhs_z, GMP_RNDN);  // temp   = z*z;
      mpfr_add(store, store, temp, GMP_RNDN);  // store += temp;
      mpfr_clear(temp);
    }
  };

  struct vector_cross_op
  {
  public:
    static void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z,
                     mpfr_t lhs_x,   mpfr_t lhs_y,   mpfr_t lhs_z,
                     mpfr_t rhs_x,   mpfr_t rhs_y,   mpfr_t rhs_z) {
      mpfr_t temp, x, y;
      mpfr_init(temp); mpfr_init(x); mpfr_init(y);
      mpfr_mul(x, lhs_y, rhs_z, GMP_RNDN);
      mpfr_mul(temp, lhs_z, rhs_y, GMP_RNDN);
      mpfr_sub(x, x, temp, GMP_RNDN);
      mpfr_mul(y, lhs_z, rhs_x, GMP_RNDN);
      mpfr_mul(temp, lhs_x, rhs_z, GMP_RNDN);
      mpfr_sub(y, y, temp, GMP_RNDN);
      mpfr_mul(store_z, lhs_x, rhs_y, GMP_RNDN);
      mpfr_mul(temp, lhs_y, rhs_x, GMP_RNDN);
      mpfr_sub(store_z, store_z, temp, GMP_RNDN);
      mpfr_set(store_y, y, GMP_RNDN);
      mpfr_set(store_x, x, GMP_RNDN);
      mpfr_clear(temp); mpfr_clear(x); mpfr_clear(y);
    }
  };

  // Unary operators

  template <int m, int d, int t>
  inline vector_proxy<m, d, t, vv_proxy<mpfr_t, vector_pos_op> >
  operator+(const vector_units<m, d, t>& n)
  { return vector_proxy<m, d, t, vv_proxy<mpfr_t, vector_pos_op> >(n); }

  template <int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, vv_proxy<op, vector_pos_op> >
  operator+(const vector_proxy<m, d, t, op>& n)
  { return vector_proxy<m, d, t, vv_proxy<op, vector_pos_op> >(n); }

  template <int m, int d, int t>
  inline vector_proxy<m, d, t, vv_proxy<mpfr_t, vector_neg_op> >
  operator-(const vector_units<m, d, t>& n)
  { return vector_proxy<m, d, t, vv_proxy<mpfr_t, vector_neg_op> >(n); }

  template <int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, vv_proxy<op, vector_neg_op> >
  operator-(const vector_proxy<m, d, t, op>& n)
  { return vector_proxy<m, d, t, vv_proxy<op, vector_neg_op> >(n); }

  template <int m, int d, int t>
  inline scalar_proxy<m, d, t, sv_proxy<mpfr_t, scalar_norm_op> >
  norm(const vector_units<m, d, t>& n)
  { return scalar_proxy<m, d, t, sv_proxy<mpfr_t, scalar_norm_op> >(n); }

  template <int m, int d, int t, typename op>
  inline scalar_proxy<m, d, t, sv_proxy<op, scalar_norm_op> >
  norm(const vector_proxy<m, d, t, op>& n)
  { return scalar_proxy<m, d, t, sv_proxy<op, scalar_norm_op> >(n); }

  template <int m, int d, int t>
  inline vector_proxy<0, 0, 0, vv_proxy<mpfr_t, vector_normalized_op> >
  normalized(const vector_units<m, d, t>& n)
  { return vector_proxy<0, 0, 0, vv_proxy<mpfr_t, vector_normalized_op> >(n); }

  template <int m, int d, int t, typename op>
  inline vector_proxy<0, 0, 0, vv_proxy<op, vector_normalized_op> >
  normalized(const vector_proxy<m, d, t, op>& n)
  { return vector_proxy<0, 0, 0, vv_proxy<op, vector_normalized_op> >(n); }

  // Addition operators

  template <int m, int d, int t>
  inline vector_proxy<m, d, t, vvv_proxy<mpfr_t, mpfr_t, vector_add_op> >
  operator+(const vector_units<m, d, t>& lhs,
            const vector_units<m, d, t>& rhs)
  { return vector_proxy<m, d, t,
                        vvv_proxy<mpfr_t, mpfr_t, vector_add_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, vvv_proxy<mpfr_t, op, vector_add_op> >
  operator+(const vector_units<m, d, t>& lhs,
            const vector_proxy<m, d, t, op>& rhs)
  { return vector_proxy<m, d, t,
                        vvv_proxy<mpfr_t, op, vector_add_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, vvv_proxy<op, mpfr_t, vector_add_op> >
  operator+(const vector_proxy<m, d, t, op>& lhs,
            const vector_units<m, d, t>& rhs)
  { return vector_proxy<m, d, t,
                        vvv_proxy<op, mpfr_t, vector_add_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op1, typename op2>
  inline vector_proxy<m, d, t, vvv_proxy<op1, op2, vector_add_op> >
  operator+(const vector_proxy<m, d, t, op1>& lhs,
            const vector_proxy<m, d, t, op2>& rhs)
  { return vector_proxy<m, d, t,
                        vvv_proxy<op1, op2, vector_add_op> >(lhs, rhs); }

  // Subtraction operators

  template <int m, int d, int t>
  inline vector_proxy<m, d, t,
                      vvv_proxy<mpfr_t, mpfr_t, vector_sub_op> >
  operator-(const vector_units<m, d, t>& lhs,
            const vector_units<m, d, t>& rhs)
  { return vector_proxy<m, d, t,
                        vvv_proxy<mpfr_t, mpfr_t, vector_sub_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, vvv_proxy<mpfr_t, op, vector_sub_op> >
  operator-(const vector_units<m, d, t>& lhs,
            const vector_proxy<m, d, t, op>& rhs)
  { return vector_proxy<m, d, t,
                        vvv_proxy<mpfr_t, op, vector_sub_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op>
  inline vector_proxy<m, d, t, vvv_proxy<op, mpfr_t, vector_sub_op> >
  operator-(const vector_proxy<m, d, t, op>& lhs,
            const vector_units<m, d, t>& rhs)
  { return vector_proxy<m, d, t,
                        vvv_proxy<op, mpfr_t, vector_sub_op> >(lhs, rhs); }

  template <int m, int d, int t, typename op1, typename op2>
  inline vector_proxy<m, d, t, vvv_proxy<op1, op2, vector_sub_op> >
  operator-(const vector_proxy<m, d, t, op1>& lhs,
            const vector_proxy<m, d, t, op2>& rhs)
  { return vector_proxy<m, d, t,
                        vvv_proxy<op1, op2, vector_sub_op> >(lhs, rhs); }

  // Multiplication operators

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvs_proxy<mpfr_t, mpfr_t, vector_mul_op> >
  operator*(const vector_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvs_proxy<mpfr_t, mpfr_t, vector_mul_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvs_proxy<mpfr_t, mpfr_t, vector_mul_op> >
  operator*(const scalar_units<m1, d1, t1>& lhs,
            const vector_units<m2, d2, t2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvs_proxy<mpfr_t, mpfr_t, vector_mul_op> >(rhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvs_proxy<mpfr_t, op, vector_mul_op> >
  operator*(const vector_units<m1, d1, t1>& lhs,
            const scalar_proxy<m2, d2, t2, op>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvs_proxy<mpfr_t, op, vector_mul_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvs_proxy<mpfr_t, op, vector_mul_op> >
  operator*(const scalar_units<m1, d1, t1>& lhs,
            const vector_proxy<m2, d2, t2, op>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvs_proxy<mpfr_t, op, vector_mul_op> >(rhs, lhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvs_proxy<op, mpfr_t, vector_mul_op> >
  operator*(const vector_proxy<m1, d1, t1, op>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvs_proxy<op, mpfr_t, vector_mul_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvs_proxy<op, mpfr_t, vector_mul_op> >
  operator*(const scalar_proxy<m1, d1, t1, op>& lhs,
            const vector_units<m2, d2, t2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvs_proxy<op, mpfr_t, vector_mul_op> >(rhs, lhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2,
	    typename op1, typename op2>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvs_proxy<op1, op2, vector_mul_op> >
  operator*(const vector_proxy<m1, d1, t1, op1>& lhs,
            const scalar_proxy<m2, d2, t2, op2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvs_proxy<op1, op2, vector_mul_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2,
	    typename op1, typename op2>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvs_proxy<op1, op2, vector_mul_op> >
  operator*(const scalar_proxy<m1, d1, t1, op1>& lhs,
            const vector_proxy<m2, d2, t2, op2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvs_proxy<op1, op2, vector_mul_op> >(rhs, lhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      svv_proxy<mpfr_t, mpfr_t, scalar_dot_op> >
  dot(const vector_units<m1, d1, t1>& lhs, const vector_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        svv_proxy<mpfr_t, mpfr_t, scalar_dot_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      svv_proxy<mpfr_t, op, scalar_dot_op> >
  dot(const vector_units<m1, d1, t1>& lhs,
      const vector_proxy<m2, d2, t2, op>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        svv_proxy<mpfr_t, op, scalar_dot_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      svv_proxy<op, mpfr_t, scalar_dot_op> >
  dot(const vector_proxy<m1, d1, t1, op>& lhs,
      const vector_units<m2, d2, t2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        svv_proxy<op, mpfr_t, scalar_dot_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2,
	    typename op1, typename op2>
  inline scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                      svv_proxy<op1, op2, scalar_dot_op> >
  dot(const vector_proxy<m1, d1, t1, op1>& lhs,
      const vector_proxy<m2, d2, t2, op2>& rhs)
  { return scalar_proxy<m1 + m2, d1 + d2, t1 + t2,
                        svv_proxy<op1, op2, scalar_dot_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvv_proxy<mpfr_t, mpfr_t, vector_cross_op> >
  cross(const vector_units<m1, d1, t1>& lhs,
        const vector_units<m2, d2, t2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvv_proxy<mpfr_t, mpfr_t,
                                  vector_cross_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvv_proxy<mpfr_t, op, vector_cross_op> >
  cross(const vector_units<m1, d1, t1>& lhs,
        const vector_proxy<m2, d2, t2, op>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvv_proxy<mpfr_t, op, vector_cross_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvv_proxy<op, mpfr_t, vector_cross_op> >
  cross(const vector_proxy<m1, d1, t1, op>& lhs,
        const vector_units<m2, d2, t2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvv_proxy<op, mpfr_t, vector_cross_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2,
	    typename op1, typename op2>
  inline vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                      vvv_proxy<op1, op2, vector_cross_op> >
  cross(const vector_proxy<m1, d1, t1, op1>& lhs,
        const vector_proxy<m2, d2, t2, op2>& rhs)
  { return vector_proxy<m1 + m2, d1 + d2, t1 + t2,
                        vvv_proxy<op1, op2, vector_cross_op> >(lhs, rhs); }

  // Division operators

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline vector_proxy<m1 - m2, d1 - d2, t1 - t2,
                      vvs_proxy<mpfr_t, mpfr_t, vector_div_op> >
  operator/(const vector_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return vector_proxy<m1 - m2, d1 - d2, t1 - t2,
                        vvs_proxy<mpfr_t, mpfr_t, vector_div_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline vector_proxy<m1 - m2, d1 - d2, t1 - t2,
                      vvs_proxy<mpfr_t, op, vector_div_op> >
  operator/(const vector_units<m1, d1, t1>& lhs,
            const scalar_proxy<m2, d2, t2, op>& rhs)
  { return vector_proxy<m1 - m2, d1 - d2, t1 - t2,
                        vvs_proxy<mpfr_t, op, vector_div_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2, typename op>
  inline vector_proxy<m1 - m2, d1 - d2, t1 - t2,
                      vvs_proxy<op, mpfr_t, vector_div_op> >
  operator/(const vector_proxy<m1, d1, t1, op>& lhs,
            const scalar_units<m2, d2, t2>& rhs)
  { return vector_proxy<m1 - m2, d1 - d2, t1 - t2,
                        vvs_proxy<op, mpfr_t, vector_div_op> >(lhs, rhs); }

  template <int m1, int m2, int d1, int d2, int t1, int t2,
	    typename op1, typename op2>
  inline vector_proxy<m1 - m2, d1 - d2, t1 - t2,
                      vvs_proxy<op1, op2, vector_div_op> >
  operator/(const vector_proxy<m1, d1, t1, op1>& lhs,
            const scalar_proxy<m2, d2, t2, op2>& rhs)
  { return vector_proxy<m1 - m2, d1 - d2, t1 - t2,
                        vvs_proxy<op1, op2, vector_div_op> >(lhs, rhs); }
}

#endif // CAROM_VECTOR_OPS_HPP
