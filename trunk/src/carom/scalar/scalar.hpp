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
 * scalar/scalar.hpp                                                     *
 * =================                                                     *
 *                                                                       *
 * The actual scalar data-type.                                          *
 *************************************************************************/

#ifndef CAROM_SCALAR_SCALAR_HPP
#define CAROM_SCALAR_SCALAR_HPP

#include <mpfr.h>
#include <cstdlib>
#include <string>

namespace carom
{
  // Workaround for member template specialization
  template<typename T> const T scalar_to(mpfr_t n);

  template<int m, int d, int t>
  class scalar_units
  {
    template<int m2, int d2, int t2, typename op> friend class scalar_proxy;

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
    explicit scalar_units(signed char n)
    { mpfr_init_set_si(m_fp, n, GMP_RNDN); }
    explicit scalar_units(signed short n)
    { mpfr_init_set_si(m_fp, n, GMP_RNDN); }
    explicit scalar_units(signed int n)
    { mpfr_init_set_si(m_fp, n, GMP_RNDN); }
    explicit scalar_units(long n)
    { mpfr_init_set_si(m_fp, n, GMP_RNDN); }
    explicit scalar_units(unsigned char n)
    { mpfr_init_set_ui(m_fp, n, GMP_RNDN); }
    explicit scalar_units(unsigned short n)
    { mpfr_init_set_ui(m_fp, n, GMP_RNDN); }
    explicit scalar_units(unsigned int n)
    { mpfr_init_set_ui(m_fp, n, GMP_RNDN); }
    explicit scalar_units(unsigned long n)
    { mpfr_init_set_ui(m_fp, n, GMP_RNDN); }

    explicit scalar_units(const char* str, int base = 10)
    { mpfr_init_set_str(m_fp, str, base, GMP_RNDN); }

    explicit scalar_units(const std::string& str, int base = 10)
    { mpfr_init_set_str(m_fp, str.c_str(), base, GMP_RNDN); }

    template<typename op> scalar_units(const scalar_proxy<m, d, t, op>& proxy)
    { mpfr_init(m_fp); proxy.eval(m_fp); }
    scalar_units(const scalar_units<m, d, t>& n)
    { mpfr_init_set(m_fp, n.m_fp, GMP_RNDN); }
    ~scalar_units() { mpfr_clear(m_fp); }

    template<typename op>
    scalar_units& operator=(const scalar_proxy<m, d, t, op>& proxy)
    { proxy.eval(m_fp); return *this; }

    scalar_units& operator=(const scalar_units<m, d, t>& rhs)
    { mpfr_set(m_fp, rhs.m_fp, GMP_RNDN); return *this; }
    scalar_units& operator+=(const scalar_units<m, d, t>& rhs)
    { mpfr_add(m_fp, m_fp, rhs.m_fp, GMP_RNDN); return *this; }
    scalar_units& operator-=(const scalar_units<m, d, t>& rhs)
    { mpfr_sub(m_fp, m_fp, rhs.m_fp, GMP_RNDN); return *this; }
    scalar_units& operator*=(const scalar_units<0, 0, 0>& rhs)
    { mpfr_mul(m_fp, m_fp, rhs.m_fp, GMP_RNDN); return *this; }

    scalar_units& operator/=(const scalar_units<0, 0, 0>& rhs) {
      if (mpfr_zero_p(rhs.m_fp)) { std::abort(); }
      mpfr_div(m_fp, m_fp, rhs.m_fp, GMP_RNDN); return *this;
    }

    // This function converts the scalar to any numeric type. In order to use
    // complete template specialization, which is impossible in this case for
    // member templates, we relegate the work to a specialized helper function.
    template<typename T> const T to() const { return scalar_to<T>(m_fp); }

  private:
    mutable mpfr_t m_fp;
  };

  inline unsigned long precision() { return mpfr_get_default_prec(); }
  inline void precision(unsigned long prec) { mpfr_set_default_prec(prec); }

  template<>
  inline const float scalar_to<float>(mpfr_t n)
  { return mpfr_get_d(n, GMP_RNDN); }
  template<>
  inline const double scalar_to<double>(mpfr_t n)
  { return mpfr_get_d(n, GMP_RNDN); }
  template<>
  inline const long double scalar_to<long double>(mpfr_t n)
  { return mpfr_get_ld(n, GMP_RNDN); }

  template<>
  inline const signed char scalar_to<signed char>(mpfr_t n)
  { return mpfr_get_si(n, GMP_RNDN); }
  template<>
  inline const signed short scalar_to<signed short>(mpfr_t n)
  { return mpfr_get_si(n, GMP_RNDN); }
  template<>
  inline const signed int scalar_to<signed int>(mpfr_t n)
  { return mpfr_get_si(n, GMP_RNDN); }
  template<>
  inline const signed long scalar_to<signed long>(mpfr_t n)
  { return mpfr_get_si(n, GMP_RNDN); }

  template<>
  inline const unsigned char scalar_to<unsigned char>(mpfr_t n)
  { return mpfr_get_ui(n, GMP_RNDN); }
  template<>
  inline const unsigned short scalar_to<unsigned short>(mpfr_t n)
  { return mpfr_get_ui(n, GMP_RNDN); }
  template<>
  inline const unsigned int scalar_to<unsigned int>(mpfr_t n)
  { return mpfr_get_ui(n, GMP_RNDN); }
  template<>
  inline const unsigned long scalar_to<unsigned long>(mpfr_t n)
  { return mpfr_get_ui(n, GMP_RNDN); }
}

#endif // CAROM_SCALAR_SCALAR_HPP
