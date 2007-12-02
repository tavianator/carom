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

#ifndef CAROM_SCALAR_HPP
#define CAROM_SCALAR_HPP

#include <mpfr.h>
#include <boost/utility.hpp> // For boost::enable_if_c
#include <algorithm> // For std::swap

namespace carom
{
  template <int m, int d, int t> class vector_units;

  template <int m, int d, int t>
  class scalar_units
  {
    template <int m2, int d2, int t2> friend class vector_units;

    // Intentionally non-template friend functions; declared in situ. Use
    // -Wno-non-template-friends to suppress g++'s warning.
    friend bool operator< (const scalar_units<m, d, t>& lhs,
                           const scalar_units<m, d, t>& rhs)
    { return mpfr_less_p(lhs.m_fp, rhs.m_fp); }
    friend bool operator<=(const scalar_units<m, d, t>& lhs,
                           const scalar_units<m, d, t>& rhs)
    { return mpfr_lessequal_p(lhs.m_fp, rhs.m_fp); }
    friend bool operator> (const scalar_units<m, d, t>& lhs,
                           const scalar_units<m, d, t>& rhs)
    { return mpfr_greater_p(lhs.m_fp, rhs.m_fp); }
    friend bool operator>=(const scalar_units<m, d, t>& lhs,
                           const scalar_units<m, d, t>& rhs)
    { return mpfr_greaterequal_p(lhs.m_fp, rhs.m_fp); }
    friend bool operator==(const scalar_units<m, d, t>& lhs,
                           const scalar_units<m, d, t>& rhs)
    { return mpfr_equal_p(lhs.m_fp, rhs.m_fp); }
    friend bool operator!=(const scalar_units<m, d, t>& lhs,
                           const scalar_units<m, d, t>& rhs)
    { return !mpfr_equal_p(lhs.m_fp, rhs.m_fp); }

  public:
    scalar_units() { mpfr_init(m_fp); }
    template <typename T>
    scalar_units(T n) { mpfr_init(m_fp); mpfr_from(m_fp, n); }
    scalar_units(const scalar_units<m, d, t>& n)
    { mpfr_init_set(m_fp, n.m_fp, GMP_RNDN); }
#if 0
    scalar_units(scalar_units<m, d, t>&& n) : m_fp(n.m_fp) { n.m_fp = 0; }
#endif
    ~scalar_units() { if (m_fp != 0) { mpfr_clear(m_fp); } }

    template <typename T>
    scalar_units& operator=(T n) { mpfr_from(m_fp, n); return *this; }
    scalar_units& operator=(const scalar_units<m, d, t>& n)
    { mpfr_set(m_fp, n.m_fp, GMP_RNDN); return *this; }
#if 0
    scalar_units& operator=(scalar_units<m, d, t>&& n)
    { std::swap(m_fp, n.m_fp); return *this; }
#endif

    scalar_units& operator+=(const scalar_units<m, d, t>& n)
    { mpfr_add(m_fp, m_fp, n.m_fp, GMP_RNDN); return *this; }
    scalar_units& operator-=(const scalar_units<m, d, t>& n)
    { mpfr_sub(m_fp, m_fp, n.m_fp, GMP_RNDN); return *this; }
    scalar_units& operator*=(const scalar_units<0, 0, 0>& n)
    { mpfr_mul(m_fp, m_fp, n.m_fp, GMP_RNDN); return *this; }
    scalar_units& operator/=(const scalar_units<0, 0, 0>& n)
    { mpfr_div(m_fp, m_fp, n.m_fp, GMP_RNDN); return *this; }

    mpfr_ptr mpfr() const { return m_fp; }

    template <typename T> T to() const { return mpfr_to<T>(m_fp); }

  private:
    mutable mpfr_t m_fp;
  };

  // Operators

  inline scalar_units<0, 0, 0> pi() {
    scalar_units<0, 0, 0> r;
    mpfr_const_pi(r.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t>
  inline scalar_units<m, d, t>
  operator+(const scalar_units<m, d, t>& n) {
    return n;
  }

  template <int m, int d, int t>
  inline scalar_units<m, d, t>
  operator-(const scalar_units<m, d, t>& n) {
    scalar_units<m, d, t> r;
    mpfr_neg(r.mpfr(), n.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t>
  inline scalar_units<m/2, d/2, t/2>
  sqrt(const scalar_units<m, d, t>& n) {
    // Use TMP to ensure that m, d, and t are even
    typedef typename boost::enable_if_c<m%2 == 0>::type m_even;
    typedef typename boost::enable_if_c<d%2 == 0>::type d_even;
    typedef typename boost::enable_if_c<t%2 == 0>::type t_even;

    scalar_units<m/2, d/2, t/2> r;
    mpfr_sqrt(r.mpfr(), n.mpfr(), GMP_RNDN);
    return r;
  }

  inline scalar_units<0, 0, 0>
  sin(const scalar_units<0, 0, 0>& n) {
    scalar_units<0, 0, 0> r;
    mpfr_sin(r.mpfr(), n.mpfr(), GMP_RNDN);
    return r;
  }

  inline scalar_units<0, 0, 0>
  cos(const scalar_units<0, 0, 0>& n) {
    scalar_units<0, 0, 0> r;
    mpfr_cos(r.mpfr(), n.mpfr(), GMP_RNDN);
    return r;
  }

  inline scalar_units<0, 0, 0>
  tan(const scalar_units<0, 0, 0>& n) {
    scalar_units<0, 0, 0> r;
    mpfr_tan(r.mpfr(), n.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t>
  inline scalar_units<m, d, t>
  operator+(const scalar_units<m, d, t>& lhs,
            const scalar_units<m, d, t>& rhs) {
    scalar_units<m, d, t> r;
    mpfr_add(r.mpfr(), lhs.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <typename T>
  inline scalar_units<0, 0, 0>
  operator+(const scalar_units<0, 0, 0>& lhs, T rhs) {
    scalar_units<0, 0, 0> r(rhs);
    mpfr_add(r.mpfr(), lhs.mpfr(), r.mpfr(), GMP_RNDN);
    return r;
  }

  template <typename T>
  inline scalar_units<0, 0, 0>
  operator+(T lhs, const scalar_units<0, 0, 0>& rhs) {
    scalar_units<0, 0, 0> r(lhs);
    mpfr_add(r.mpfr(), r.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  // Disambiguate the scalar_units<0, 0, 0> + scalar_units<0, 0, 0> case
  inline scalar_units<0, 0, 0>
  operator+(const scalar_units<0, 0, 0>& lhs,
            const scalar_units<0, 0, 0>& rhs) {
    scalar_units<0, 0, 0> r;
    mpfr_add(r.mpfr(), lhs.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t>
  inline scalar_units<m, d, t>
  operator-(const scalar_units<m, d, t>& lhs,
            const scalar_units<m, d, t>& rhs) {
    scalar_units<m, d, t> r;
    mpfr_sub(r.mpfr(), lhs.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <typename T>
  inline scalar_units<0, 0, 0>
  operator-(const scalar_units<0, 0, 0>& lhs, T rhs) {
    scalar_units<0, 0, 0> r(rhs);
    mpfr_sub(r.mpfr(), lhs.mpfr(), r.mpfr(), GMP_RNDN);
    return r;
  }

  template <typename T>
  inline scalar_units<0, 0, 0>
  operator-(T lhs, const scalar_units<0, 0, 0>& rhs) {
    scalar_units<0, 0, 0> r(lhs);
    mpfr_sub(r.mpfr(), r.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline scalar_units<m1 + m2, d1 + d2, t1 + t2>
  operator*(const scalar_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs) {
    scalar_units<m1 + m2, d1 + d2, t1 + t2> r;
    mpfr_mul(r.mpfr(), lhs.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t, typename T>
  inline scalar_units<m, d, t>
  operator*(const scalar_units<m, d, t>& lhs, T rhs) {
    scalar_units<m, d, t> r(rhs);
    mpfr_mul(r.mpfr(), lhs.mpfr(), r.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t, typename T>
  inline scalar_units<m, d, t>
  operator*(T lhs, const scalar_units<m, d, t>& rhs) {
    scalar_units<m, d, t> r(lhs);
    mpfr_mul(r.mpfr(), r.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m1, int m2, int d1, int d2, int t1, int t2>
  inline scalar_units<m1 - m2, d1 - d2, t1 - t2>
  operator/(const scalar_units<m1, d1, t1>& lhs,
            const scalar_units<m2, d2, t2>& rhs) {
    scalar_units<m1 - m2, d1 - d2, t1 - t2> r;
    mpfr_div(r.mpfr(), lhs.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t, typename T>
  inline scalar_units<m, d, t>
  operator/(const scalar_units<m, d, t>& lhs, T rhs) {
    scalar_units<m, d, t> r(rhs);
    mpfr_div(r.mpfr(), lhs.mpfr(), r.mpfr(), GMP_RNDN);
    return r;
  }

  template <int m, int d, int t, typename T>
  inline scalar_units<m, d, t>
  operator/(T lhs, const scalar_units<m, d, t>& rhs) {
    scalar_units<m, d, t> r(lhs);
    mpfr_div(r.mpfr(), r.mpfr(), rhs.mpfr(), GMP_RNDN);
    return r;
  }

  // Convenient typedefs
  typedef scalar_units<0, 0, 0>  scalar;
  typedef scalar_units<0, 0, 0>  scalar_angle;
  typedef scalar_units<1, 0, 0>  scalar_mass;
  typedef scalar_units<0, 1, 0>  scalar_distance;
  typedef scalar_units<0, 0, 1>  scalar_time;
  typedef scalar_units<0, 1, -1> scalar_speed;
  typedef scalar_units<1, 1, -1> scalar_momentum;
  typedef scalar_units<0, 1, -2> scalar_acceleration;
  typedef scalar_units<1, 1, -2> scalar_force;
  typedef scalar_units<1, 2, 0>  scalar_moment_of_inertia;
  typedef scalar_units<0, 0, -1> scalar_angular_speed;
  typedef scalar_units<1, 2, -1> scalar_angular_momentum;
  typedef scalar_units<0, 0, -2> scalar_angular_acceleration;
  typedef scalar_units<1, 2, -2> scalar_torque;
  typedef scalar_units<1, 2, -2> scalar_energy;
  typedef scalar_units<1, 2, -3> scalar_power;
}

#endif // CAROM_SCALAR_HPP
