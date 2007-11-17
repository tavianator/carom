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

#ifndef CAROM_SCALAR_SCALAR_HPP
#define CAROM_SCALAR_SCALAR_HPP

#include <mpfr.h>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace carom
{
  template <int m, int d, int t> class vector_units;

  template <int m, int d, int t>
  class scalar_units
  {
    template <int m2, int d2, int t2, typename op> friend class scalar_proxy;
    template <int m2, int d2, int t2>              friend class vector_units;

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
    scalar_units(const T& n)
    { mpfr_init(m_fp); mpfr_from(m_fp, n); }

    template <typename op>
    scalar_units(const scalar_proxy<m, d, t, op>& proxy)
    { mpfr_init(m_fp); proxy.eval(m_fp); }

    scalar_units(const scalar_units<m, d, t>& n)
    { mpfr_init_set(m_fp, n.m_fp, GMP_RNDN); }

    ~scalar_units() { mpfr_clear(m_fp); }

    template <typename T>
    scalar_units& operator=(const T& n) { mpfr_from(m_fp, n); }

    template <typename op>
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
      if (mpfr_zero_p(rhs.m_fp)) {
        throw std::domain_error("Division by zero");
      }

      mpfr_div(m_fp, m_fp, rhs.m_fp, GMP_RNDN); return *this;
    }

    template <typename T> const T to() const { return mpfr_to<T>(m_fp); }

  private:
    mutable mpfr_t m_fp;
  };
}

#endif // CAROM_SCALAR_SCALAR_HPP
