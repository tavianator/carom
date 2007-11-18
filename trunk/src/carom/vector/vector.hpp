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

#ifndef CAROM_VECTOR_VECTOR_HPP
#define CAROM_VECTOR_VECTOR_HPP

#include <mpfr.h>

namespace carom
{
  template <int m, int d, int t>
  class vector_units
  {
    template <int m2, int d2, int t2, typename op> friend class scalar_proxy;
    template <int m2, int d2, int t2, typename op> friend class vector_proxy;

    // Intentionally non-template friend functions; declared in situ. Use
    // -Wno-non-template-friends to suppress g++'s warning.
    friend bool operator==(const vector_units<m, d, t>& lhs,
                           const vector_units<m, d, t>& rhs) {
      return mpfr_equal_p(lhs.m_x, rhs.m_y) &&
             mpfr_equal_p(lhs.m_y, rhs.m_y) &&
             mpfr_equal_p(lhs.m_z, rhs.m_z);
    }

    friend bool operator!=(const vector_units<m, d, t>& lhs,
                           const vector_units<m, d, t>& rhs) {
      return !(mpfr_equal_p(lhs.m_x, rhs.m_y) &&
               mpfr_equal_p(lhs.m_y, rhs.m_y) &&
               mpfr_equal_p(lhs.m_z, rhs.m_z));
    }

  public:
    vector_units() { mpfr_init(m_x); mpfr_init(m_y); mpfr_init(m_z); }

    vector_units(void*) { // For constructs like vector x = 0; if (x == 0) { }
      mpfr_init(m_x); mpfr_init(m_y); mpfr_init(m_z);
      mpfr_set_ui(m_x, 0, GMP_RNDN);
      mpfr_set_ui(m_y, 0, GMP_RNDN);
      mpfr_set_ui(m_z, 0, GMP_RNDN);
    }

    template <typename T, typename U, typename V>
    vector_units(const T& x, const U& y, const V& z) {
      mpfr_init(m_x); mpfr_init(m_y); mpfr_init(m_z);
      mpfr_from(m_x, x); mpfr_from(m_y, y); mpfr_from(m_z, z);
    }

    template <typename op>
    vector_units(const vector_proxy<m, d, t, op>& proxy) {
      mpfr_init(m_x); mpfr_init(m_y); mpfr_init(m_z);
      proxy.eval(m_x, m_y, m_z);
    }

    vector_units(const vector_units<m, d, t>& n) {
      mpfr_init_set(m_x, n.m_x, GMP_RNDN);
      mpfr_init_set(m_y, n.m_y, GMP_RNDN);
      mpfr_init_set(m_z, n.m_z, GMP_RNDN);
    }

    ~vector_units() { mpfr_clear(m_x); mpfr_clear(m_y); mpfr_clear(m_z); }

    template <typename op>
    vector_units& operator=(const vector_proxy<m, d, t, op>& proxy)
    { proxy.eval(m_x, m_y, m_z); return *this; }

    vector_units& operator=(const vector_units<m, d, t>& rhs) {
      mpfr_set(m_x, rhs.m_x, GMP_RNDN);
      mpfr_set(m_y, rhs.m_y, GMP_RNDN);
      mpfr_set(m_z, rhs.m_z, GMP_RNDN);
      return *this;
    }

    vector_units& operator+=(const vector_units<m, d, t>& rhs) {
      mpfr_add(m_x, m_x, rhs.m_x, GMP_RNDN);
      mpfr_add(m_y, m_y, rhs.m_y, GMP_RNDN);
      mpfr_add(m_z, m_z, rhs.m_z, GMP_RNDN);
      return *this;
    }

    vector_units& operator-=(const vector_units<m, d, t>& rhs) {
      mpfr_sub(m_x, m_x, rhs.m_x, GMP_RNDN);
      mpfr_sub(m_y, m_y, rhs.m_y, GMP_RNDN);
      mpfr_sub(m_z, m_z, rhs.m_z, GMP_RNDN);
      return *this;
    }

    vector_units& operator*=(const scalar_units<0, 0, 0>& rhs) {
      mpfr_mul(m_x, m_x, rhs.m_fp, GMP_RNDN);
      mpfr_mul(m_y, m_y, rhs.m_fp, GMP_RNDN);
      mpfr_mul(m_z, m_z, rhs.m_fp, GMP_RNDN);
      return *this;
    }

    vector_units& operator/=(const scalar_units<0, 0, 0>& rhs) {
      mpfr_div(m_x, m_x, rhs.m_fp, GMP_RNDN);
      mpfr_div(m_y, m_y, rhs.m_fp, GMP_RNDN);
      mpfr_div(m_y, m_y, rhs.m_fp, GMP_RNDN);
      return *this;
    }

    const scalar_units<m, d, t> x() {
      scalar_units<m, d, t> x;
      mpfr_set(x.m_fp, m_x, GMP_RNDN);
      return x;
    }

    const scalar_units<m, d, t> y() {
      scalar_units<m, d, t> y;
      mpfr_set(y.m_fp, m_y, GMP_RNDN);
      return y;
    }

    const scalar_units<m, d, t> z() {
      scalar_units<m, d, t> z;
      mpfr_set(z.m_fp, m_z, GMP_RNDN);
      return z;
    }

  private:
    mutable mpfr_t m_x, m_y, m_z;
  };
}

#endif // CAROM_VECTOR_VECTOR_HPP
