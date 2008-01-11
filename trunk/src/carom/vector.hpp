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

#ifndef CAROM_VECTOR_HPP
#define CAROM_VECTOR_HPP

#include <mpfr.h>

namespace carom
{
  template <int m, int d, int t>
  class vector_units
  {
    // Intentionally non-template friend functions; declared in situ. Use
    // -Wno-non-template-friends to suppress g++'s warning.

    friend bool operator==(const vector_units<m, d, t>& lhs,
                           const vector_units<m, d, t>& rhs) {
      return mpfr_equal_p(lhs.m_x, rhs.m_x) &&
             mpfr_equal_p(lhs.m_y, rhs.m_y) &&
             mpfr_equal_p(lhs.m_z, rhs.m_z);
    }

    friend bool operator!=(const vector_units<m, d, t>& lhs,
                           const vector_units<m, d, t>& rhs) {
      return !(mpfr_equal_p(lhs.m_x, rhs.m_x) &&
               mpfr_equal_p(lhs.m_y, rhs.m_y) &&
               mpfr_equal_p(lhs.m_z, rhs.m_z));
    }

  public:
    vector_units() { init(); }

    // For constructs like vector v = 0; if (v == 0) { }
    vector_units(void*) {
      init();
      mpfr_set_ui(m_x, 0, GMP_RNDN);
      mpfr_set_ui(m_y, 0, GMP_RNDN);
      mpfr_set_ui(m_z, 0, GMP_RNDN);
    }

    vector_units(const scalar_units<m, d, t>& x,
                 const scalar_units<m, d, t>& y,
                 const scalar_units<m, d, t>& z) {
      init();
      mpfr_set(m_x, x.mpfr(), GMP_RNDN);
      mpfr_set(m_y, y.mpfr(), GMP_RNDN);
      mpfr_set(m_z, z.mpfr(), GMP_RNDN);
    }

    vector_units(const vector_units<m, d, t>& n) {
      init();
      mpfr_set(m_x, n.m_x, GMP_RNDN);
      mpfr_set(m_y, n.m_y, GMP_RNDN);
      mpfr_set(m_z, n.m_z, GMP_RNDN);
    }

    ~vector_units() {
      pool().release(m_x);
      pool().release(m_y);
      pool().release(m_z);
    }

    vector_units& operator=(const vector_units<m, d, t>& n) {
      update_precision(); // In case precision has changed
      mpfr_set(m_x, n.m_x, GMP_RNDN);
      mpfr_set(m_y, n.m_y, GMP_RNDN);
      mpfr_set(m_z, n.m_z, GMP_RNDN);
      return *this;
    }

    vector_units& operator+=(const vector_units<m, d, t>& n) {
      update_precision();
      mpfr_add(m_x, m_x, n.m_x, GMP_RNDN);
      mpfr_add(m_y, m_y, n.m_y, GMP_RNDN);
      mpfr_add(m_z, m_z, n.m_z, GMP_RNDN);
      return *this;
    }

    vector_units& operator-=(const vector_units<m, d, t>& n) {
      update_precision();
      mpfr_sub(m_x, m_x, n.m_x, GMP_RNDN);
      mpfr_sub(m_y, m_y, n.m_y, GMP_RNDN);
      mpfr_sub(m_z, m_z, n.m_z, GMP_RNDN);
      return *this;
    }

    vector_units& operator*=(const scalar_units<0, 0, 0>& n) {
      update_precision();
      mpfr_mul(m_x, m_x, n.mpfr(), GMP_RNDN);
      mpfr_mul(m_y, m_y, n.mpfr(), GMP_RNDN);
      mpfr_mul(m_z, m_z, n.mpfr(), GMP_RNDN);
      return *this;
    }

    vector_units& operator/=(const scalar_units<0, 0, 0>& n) {
      update_precision();
      mpfr_div(m_x, m_x, n.mpfr(), GMP_RNDN);
      mpfr_div(m_y, m_y, n.mpfr(), GMP_RNDN);
      mpfr_div(m_z, m_z, n.mpfr(), GMP_RNDN);
      return *this;
    }

    scalar_units<m, d, t> x() const
    { scalar_units<m, d, t> r; mpfr_set(r.mpfr(), m_x, GMP_RNDN); return r; }
    scalar_units<m, d, t> y() const
    { scalar_units<m, d, t> r; mpfr_set(r.mpfr(), m_y, GMP_RNDN); return r; }
    scalar_units<m, d, t> z() const
    { scalar_units<m, d, t> r; mpfr_set(r.mpfr(), m_z, GMP_RNDN); return r; }

    mpfr_ptr mpfr_x() const { return m_x; }
    mpfr_ptr mpfr_y() const { return m_y; }
    mpfr_ptr mpfr_z() const { return m_z; }

  private:
    mpfr_ptr m_x, m_y, m_z;

    void init() {
      m_x = pool().acquire();
      m_y = pool().acquire();
      m_z = pool().acquire();
    }

    void update_precision() {
      mpfr_prec_round(m_x, precision(), GMP_RNDN);
      mpfr_prec_round(m_y, precision(), GMP_RNDN);
      mpfr_prec_round(m_z, precision(), GMP_RNDN);
    }
  };

  // A method of bypassing the unit-correctness system, needed in some cases

  template <int m1, int d1, int t1, int m2, int d2, int t2>
  inline vector_units<m1, d1, t1> convert(const vector_units<m2, d2, t2>& n) {
    vector_units<m1, d1, t1> r;
    mpfr_set(r.mpfr_x(), n.mpfr_x(), GMP_RNDN);
    mpfr_set(r.mpfr_y(), n.mpfr_y(), GMP_RNDN);
    mpfr_set(r.mpfr_z(), n.mpfr_z(), GMP_RNDN);
    return r;
  }

  template <typename T, int m, int d, int t>
  inline T convert(const vector_units<m, d, t>& n) {
    T r;
    mpfr_set(r.mpfr_x(), n.mpfr_x(), GMP_RNDN);
    mpfr_set(r.mpfr_y(), n.mpfr_y(), GMP_RNDN);
    mpfr_set(r.mpfr_z(), n.mpfr_z(), GMP_RNDN);
    return r;
  }

  // Operators

  template <int m, int d, int t>
  inline vector_units<m, d, t>
  operator+(const vector_units<m, d, t>& n) {
    return n;
  }

  template <int m, int d, int t>
  inline vector_units<m, d, t>
  operator-(const vector_units<m, d, t>& n) {
    vector_units<m, d, t> r;
    mpfr_neg(r.mpfr_x(), n.mpfr_x(), GMP_RNDN);
    mpfr_neg(r.mpfr_y(), n.mpfr_y(), GMP_RNDN);
    mpfr_neg(r.mpfr_z(), n.mpfr_z(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t>
  inline vector_units<m, d, t>
  operator+(const vector_units<m, d, t>& lhs,
            const vector_units<m, d, t>& rhs) {
    vector_units<m, d, t> r;
    mpfr_add(r.mpfr_x(), lhs.mpfr_x(), rhs.mpfr_x(), GMP_RNDN);
    mpfr_add(r.mpfr_y(), lhs.mpfr_y(), rhs.mpfr_y(), GMP_RNDN);
    mpfr_add(r.mpfr_z(), lhs.mpfr_z(), rhs.mpfr_z(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t>
  inline vector_units<m, d, t>
  operator-(const vector_units<m, d, t>& lhs,
            const vector_units<m, d, t>& rhs) {
    vector_units<m, d, t> r;
    mpfr_sub(r.mpfr_x(), lhs.mpfr_x(), rhs.mpfr_x(), GMP_RNDN);
    mpfr_sub(r.mpfr_y(), lhs.mpfr_y(), rhs.mpfr_y(), GMP_RNDN);
    mpfr_sub(r.mpfr_z(), lhs.mpfr_z(), rhs.mpfr_z(), GMP_RNDN);
    return r;
  }

  template <int m1, int d1, int t1, int m2, int d2, int t2>
  inline vector_units<m1 + m2, d1 + d2, t1 + t2>
  operator*(const vector_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs) {
    vector_units<m1 + m2, d1 + d2, t1 + t2> r;
    mpfr_mul(r.mpfr_x(), lhs.mpfr_x(), rhs.mpfr(), GMP_RNDN);
    mpfr_mul(r.mpfr_y(), lhs.mpfr_y(), rhs.mpfr(), GMP_RNDN);
    mpfr_mul(r.mpfr_z(), lhs.mpfr_z(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m1, int d1, int t1, int m2, int d2, int t2>
  inline vector_units<m1 + m2, d1 + d2, t1 + t2>
  operator*(const scalar_units<m1, d1, t1>& lhs,
            const vector_units<m2, d2, t2>& rhs) {
    vector_units<m1 + m2, d1 + d2, t1 + t2> r;
    mpfr_mul(r.mpfr_x(), lhs.mpfr(), rhs.mpfr_x(), GMP_RNDN);
    mpfr_mul(r.mpfr_y(), lhs.mpfr(), rhs.mpfr_y(), GMP_RNDN);
    mpfr_mul(r.mpfr_z(), lhs.mpfr(), rhs.mpfr_z(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t, typename T>
  inline vector_units<m, d, t>
  operator*(const vector_units<m, d, t>& lhs, T rhs) {
    vector_units<m, d, t> r;
    scalar_units<m, d, t> temp(rhs);
    mpfr_mul(r.mpfr_x(), lhs.mpfr_x(), temp.mpfr(), GMP_RNDN);
    mpfr_mul(r.mpfr_y(), lhs.mpfr_y(), temp.mpfr(), GMP_RNDN);
    mpfr_mul(r.mpfr_z(), lhs.mpfr_z(), temp.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t, typename T>
  inline vector_units<m, d, t>
  operator*(T lhs, const vector_units<m, d, t>& rhs) {
    vector_units<m, d, t> r;
    scalar_units<m, d, t> temp(lhs);
    mpfr_mul(r.mpfr_x(), temp.mpfr(), rhs.mpfr_x(), GMP_RNDN);
    mpfr_mul(r.mpfr_y(), temp.mpfr(), rhs.mpfr_y(), GMP_RNDN);
    mpfr_mul(r.mpfr_z(), temp.mpfr(), rhs.mpfr_z(), GMP_RNDN);
    return r;
  }

  template <int m1, int d1, int t1, int m2, int d2, int t2>
  inline vector_units<m1 - m2, d1 - d2, t1 - t2>
  operator/(const vector_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs) {
    vector_units<m1 - m2, d1 - d2, t1 - t2> r;
    mpfr_div(r.mpfr_x(), lhs.mpfr_x(), rhs.mpfr(), GMP_RNDN);
    mpfr_div(r.mpfr_y(), lhs.mpfr_y(), rhs.mpfr(), GMP_RNDN);
    mpfr_div(r.mpfr_z(), lhs.mpfr_z(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t, typename T>
  inline vector_units<m, d, t>
  operator/(const vector_units<m, d, t>& lhs, T rhs) {
    vector_units<m, d, t> r;
    scalar_units<m, d, t> temp(rhs);
    mpfr_div(r.mpfr_x(), lhs.mpfr_x(), temp.mpfr(), GMP_RNDN);
    mpfr_div(r.mpfr_y(), lhs.mpfr_y(), temp.mpfr(), GMP_RNDN);
    mpfr_div(r.mpfr_z(), lhs.mpfr_z(), temp.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t>
  inline scalar_units<m, d, t>
  norm(const vector_units<m, d, t>& n) {
    scalar_units<m, d, t> r, temp;
    mpfr_sqr(temp.mpfr(), n.mpfr_x(), GMP_RNDN);            // temp = x*x;
    mpfr_sqr(r.mpfr(), n.mpfr_y(), GMP_RNDN);               // r = y*y;
    mpfr_add(temp.mpfr(), temp.mpfr(), r.mpfr(), GMP_RNDN); // temp += r;
    mpfr_sqr(r.mpfr(), n.mpfr_z(), GMP_RNDN);               // r = z*z;
    mpfr_add(temp.mpfr(), temp.mpfr(), r.mpfr(), GMP_RNDN); // temp += r;
    mpfr_sqrt(r.mpfr(), temp.mpfr(), GMP_RNDN);             // r = sqrt(temp);
    return r;
  }

  template <int m, int d, int t>
  inline vector_units<0, 0, 0>
  normalized(const vector_units<m, d, t>& n) {
    return n/norm(n);
  }

  template <int m1, int d1, int t1, int m2, int d2, int t2>
  inline scalar_units<m1 + m2, d1 + d2, t1 + t2>
  dot(const vector_units<m1, d1, t1>& lhs,
      const vector_units<m2, d2, t2>& rhs) {
    scalar_units<m1 + m2, d1 + d2, t1 + t2> r, temp;
    mpfr_mul(r.mpfr(), lhs.mpfr_x(), rhs.mpfr_x(), GMP_RNDN);
    mpfr_mul(temp.mpfr(), lhs.mpfr_y(), rhs.mpfr_y(), GMP_RNDN);
    mpfr_add(r.mpfr(), r.mpfr(), temp.mpfr(), GMP_RNDN);
    mpfr_mul(temp.mpfr(), lhs.mpfr_z(), rhs.mpfr_z(), GMP_RNDN);
    mpfr_add(r.mpfr(), r.mpfr(), temp.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m1, int d1, int t1, int m2, int d2, int t2>
  inline vector_units<m1 + m2, d1 + d2, t1 + t2>
  cross(const vector_units<m1, d1, t1>& lhs,
        const vector_units<m2, d2, t2>& rhs) {
    // xyzzy
    vector_units<m1 + m2, d1 + d2, t1 + t2> r;
    scalar_units<m1 + m2, d1 + d2, t1 + t2> temp;
    mpfr_mul(r.mpfr_x(), lhs.mpfr_y(), rhs.mpfr_z(), GMP_RNDN);
    mpfr_mul(temp.mpfr(), lhs.mpfr_z(), rhs.mpfr_y(), GMP_RNDN);
    mpfr_sub(r.mpfr_x(), r.mpfr_x(), temp.mpfr(), GMP_RNDN);

    mpfr_mul(r.mpfr_y(), lhs.mpfr_z(), rhs.mpfr_x(), GMP_RNDN);
    mpfr_mul(temp.mpfr(), lhs.mpfr_x(), rhs.mpfr_z(), GMP_RNDN);
    mpfr_sub(r.mpfr_y(), r.mpfr_y(), temp.mpfr(), GMP_RNDN);

    mpfr_mul(r.mpfr_z(), lhs.mpfr_x(), rhs.mpfr_y(), GMP_RNDN);
    mpfr_mul(temp.mpfr(), lhs.mpfr_y(), rhs.mpfr_x(), GMP_RNDN);
    mpfr_sub(r.mpfr_z(), r.mpfr_z(), temp.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m1, int d1, int t1, int m2, int d2, int t2>
  inline vector_units<m2, d2, t2>
  proj(const vector_units<m1, d1, t1>& u,
       const vector_units<m2, d2, t2>& d) {
    return u * dot(u, d)/dot(u, u);
  }

  // Convenient typedefs
  typedef vector_units<0, 0, 0>  vector;
  typedef vector_units<0, 0, 0>  vector_angle;
  typedef vector_units<0, 1, 0>  vector_displacement;
  typedef vector_units<0, 1, -1> vector_velocity;
  typedef vector_units<1, 1, -1> vector_momentum;
  typedef vector_units<0, 1, -2> vector_acceleration;
  typedef vector_units<1, 1, -2> vector_force;
  typedef vector_units<0, 0, -1> vector_angular_velocity;
  typedef vector_units<1, 2, -1> vector_angular_momentum;
  typedef vector_units<0, 0, -2> vector_angular_acceleration;
  typedef vector_units<1, 2, -2> vector_torque;
}

#endif // CAROM_VECTOR_HPP
