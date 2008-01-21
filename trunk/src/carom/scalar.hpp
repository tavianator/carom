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

namespace carom
{
  // A few notes:
  //  - scalar_units is a templated class, with (currently) three integer
  //    parameters: m, d, and t, corresponding to the exponent on the mass,
  //    distance, and time units, respectively.
  //  - Only two scalars with identicial units may be added, and when two
  //    scalars are multiplied, the exponents are added to form a new type
  //  - A method for bypassing this system is provided with the template
  //    function convert. Call convert<0, 1, -1>() to convert any scalar
  //    to a speed.
  //  - Some useful typedefs are provided for often-used units. See below.
  //
  //  - This class uses MPFR for machine-independant, arbitrary-precision,
  //    high-performance floating-point arithmetic.
  //  - Currently, all MPFR calls use GMP_RNDN as the rounding mode, though
  //    this may not always produce propper rounding.
  //  - NRVO is expected to be implemented to provide decent performance, as is
  //    function inlining.
  //  - Move symantics will provide even more optimization

  template <int m, int d, int t>
  class scalar_units
  {
    // Intentionally non-template friend functions; declared in situ. Use
    // -Wno-non-template-friends to suppress g++'s warning.
    friend bool operator<(const scalar_units<m, d, t>& lhs,
                          const scalar_units<m, d, t>& rhs)
    { return mpfr_less_p(lhs.m_fp, rhs.m_fp); }
    friend bool operator<=(const scalar_units<m, d, t>& lhs,
                           const scalar_units<m, d, t>& rhs)
    { return mpfr_lessequal_p(lhs.m_fp, rhs.m_fp); }
    friend bool operator>(const scalar_units<m, d, t>& lhs,
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
    scalar_units() { init(); }
    template <typename T>
    scalar_units(const T& n) { init(); mpfr_from(m_fp, n); }
    scalar_units(const scalar_units<m, d, t>& n)
    { init(); mpfr_set(m_fp, n.m_fp, GMP_RNDN); }
    ~scalar_units() { pool().release(m_fp); }

    template <typename T>
    scalar_units& operator=(T n) {
      update_precision(); // In case precision has changed
      mpfr_from(m_fp, n);
      return *this;
    }

    scalar_units& operator=(const scalar_units<m, d, t>& n) {
      update_precision();
      mpfr_set(m_fp, n.m_fp, GMP_RNDN);
      return *this;
    }

    scalar_units& operator+=(const scalar_units<m, d, t>& n) {
      update_precision();
      mpfr_add(m_fp, m_fp, n.m_fp, GMP_RNDN);
      return *this;
    }

    scalar_units& operator-=(const scalar_units<m, d, t>& n) {
      update_precision();
      mpfr_sub(m_fp, m_fp, n.m_fp, GMP_RNDN);
      return *this;
    }

    scalar_units& operator*=(const scalar_units<0, 0, 0>& n) {
      update_precision();
      mpfr_mul(m_fp, m_fp, n.mpfr(), GMP_RNDN);
      return *this;
    }

    scalar_units& operator/=(const scalar_units<0, 0, 0>& n) {
      update_precision();
      mpfr_div(m_fp, m_fp, n.mpfr(), GMP_RNDN);
      return *this;
    }

    mpfr_ptr mpfr() const { return m_fp; }

    template <typename T> T to() const { return mpfr_to<T>(m_fp); }

  private:
    mpfr_ptr m_fp;

    void init() { m_fp = pool().acquire(); }
    void update_precision() { mpfr_prec_round(m_fp, precision(), GMP_RNDN); }
  };

  // A method of bypassing the unit-correctness system, needed in some cases

  template <int m1, int d1, int t1, int m2, int d2, int t2>
  inline scalar_units<m1, d1, t1> convert(const scalar_units<m2, d2, t2>& n) {
    scalar_units<m1, d1, t1> r;
    mpfr_set(r.mpfr(), n.mpfr(), GMP_RNDN);
    return r;
  }

  template <typename T, int m, int d, int t>
  inline T convert(const scalar_units<m, d, t>& n) {
    T r;
    mpfr_set(r.mpfr(), n.mpfr(), GMP_RNDN);
    return r;
  }

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
  inline scalar_units<m, d, t>
  abs(const scalar_units<m, d, t>& n) {
    scalar_units<m, d, t> r;
    mpfr_abs(r.mpfr(), n.mpfr(), GMP_RNDN);
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
  inline scalar_units<0, 0, 0>
  atan2(const scalar_units<m, d, t>& x, const scalar_units<m, d, t>& y) {
    scalar_units<0, 0, 0> r;
    mpfr_atan2(r.mpfr(), x.mpfr(), y.mpfr(), GMP_RNDN);
    return r;
  }

  inline scalar_units<0, 0, 0>
  pow(const scalar_units<0, 0, 0>& b, const scalar_units<0, 0, 0>& e) {
    scalar_units<0, 0, 0> r;
    mpfr_pow(r.mpfr(), b.mpfr(), e.mpfr(), GMP_RNDN);
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

  template <int m1, int d1, int t1, int m2, int d2, int t2>
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

  template <int m1, int d1, int t1, int m2, int d2, int t2>
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
  inline scalar_units<-m, -d, -t>
  operator/(T lhs, const scalar_units<m, d, t>& rhs) {
    scalar_units<-m, -d, -t> r(lhs);
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
