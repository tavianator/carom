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
 * scalar/proxy.hpp                                                      *
 * ================                                                      *
 *                                                                       *
 * A proxy class for operations on scalars.                              *
 *************************************************************************/

#ifndef CAROM_SCALAR_PROXY_HPP
#define CAROM_SCALAR_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  // Unary scalar_proxy's

  template<int m, int d, int t, typename T, typename op>
  class scalar_proxy<m, d, t, scalar_unary_proxy<T, op> >
    : public scalar_unary_proxy<T, op>
  {
  public:
    scalar_proxy(const T& n) : scalar_unary_proxy<T, op>(n) { }
    // scalar_proxy(const scalar_proxy& proxy);
    // ~scalar_proxy();

  private:
    scalar_proxy& operator=(const scalar_proxy&);
  };

  template<int m, int d, int t, typename op>
  class scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, op> >
    : public scalar_unary_proxy<mpfr_t, op>
  {
  public:
    template<int m2, int d2, int t2>
    scalar_proxy(const scalar_units<m2, d2, t2>& n)
    : scalar_unary_proxy<mpfr_t, op>(n.m_fp) { }

    // scalar_proxy(const scalar_proxy& proxy);
    // ~scalar_proxy();

  private:
    scalar_proxy& operator=(const scalar_proxy&);
  };

  // Binary scalar_proxy's

  template<int m, int d, int t, typename T, typename U, typename op>
  class scalar_proxy<m, d, t, scalar_binary_proxy<T, U, op> >
    : public scalar_binary_proxy<T, U, op>
  {
  public:
    scalar_proxy(const T& lhs, const U& rhs)
    : scalar_binary_proxy<T, U, op>(lhs, rhs) { }

    // scalar_proxy(const scalar_proxy& proxy);
    // ~scalar_proxy();

  private:
    scalar_proxy& operator=(const scalar_proxy&);
  };

  template<int m, int d, int t, typename T, typename op>
  class scalar_proxy<m, d, t, scalar_binary_proxy<T, mpfr_t, op> >
    : public scalar_binary_proxy<T, mpfr_t, op>
  {
  public:
    template<int m2, int d2, int t2>
    scalar_proxy(const T& lhs, const scalar_units<m2, d2, t2>& rhs)
    : scalar_binary_proxy<T, mpfr_t, op>(lhs, rhs.m_fp) { }

    // scalar_proxy(const scalar_proxy& proxy);
    // ~scalar_proxy();

  private:
    scalar_proxy& operator=(const scalar_proxy&);
  };

  template<int m, int d, int t, typename T, typename op>
  class scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, T, op> >
    : public scalar_binary_proxy<mpfr_t, T, op>
  {
  public:
    template<int m2, int d2, int t2>
    scalar_proxy(const scalar_units<m2, d2, t2>& lhs, const T& rhs)
    : scalar_binary_proxy<mpfr_t, T, op>(lhs.m_fp, rhs) { }

    // scalar_proxy(const scalar_proxy& proxy);
    // ~scalar_proxy();

  private:
    scalar_proxy& operator=(const scalar_proxy&);
  };

  template<int m, int d, int t, typename op>
  class scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, mpfr_t, op> >
    : public scalar_binary_proxy<mpfr_t, mpfr_t, op>
  {
  public:
    template<int m1, int m2, int d1, int d2, int t1, int t2>
    scalar_proxy(const scalar_units<m1, d1, t1>& lhs,
                const scalar_units<m2, d2, t2>& rhs)
    : scalar_binary_proxy<mpfr_t, mpfr_t, op>(lhs.m_fp, rhs.m_fp) { }

    // scalar_proxy(const scalar_proxy& proxy);
    // ~scalar_proxy();

  private:
    scalar_proxy& operator=(const scalar_proxy&);
  };
}

#endif // CAROM_SCALAR_PROXY_HPP
