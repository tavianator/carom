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

#ifndef CAROM_VECTOR_PROXY_HPP
#define CAROM_VECTOR_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  // Unary vector_proxy's

  template<int m, int d, int t, typename T, typename op>
  class vector_proxy<m, d, t, scalar_unary_proxy<T, op> >
  {
    template<int m2, int d2, int t2, typename op2> friend class vector_proxy;

  public:
    template<int m2, int d2, int t2>
    vector_proxy(const vector_proxy<m2, d2, t2, T>& proxy)
    : m_x(proxy.m_x), m_y(proxy.m_y), m_z(proxy.m_z) { }
    // vector_proxy(const vector_proxy& proxy);
    // ~vector_proxy();

    void eval(scalar_units<m, d, t>& x,
              scalar_units<m, d, t>& y,
              scalar_units<m, d, t>& z) const
    { x = m_x; y = m_y; z = m_z; }

  private:
    scalar_proxy<m, d, t, scalar_unary_proxy<T, op> > m_x;
    scalar_proxy<m, d, t, scalar_unary_proxy<T, op> > m_y;
    scalar_proxy<m, d, t, scalar_unary_proxy<T, op> > m_z;

    vector_proxy& operator=(const vector_proxy&);
  };

  template<int m, int d, int t, typename op>
  class vector_proxy<m, d, t, scalar_unary_proxy<mpfr_t, op> >
  {
    template<int m2, int d2, int t2, typename op2> friend class vector_proxy;

  public:
    template<int m2, int d2, int t2>
    vector_proxy(const vector_units<m2, d2, t2>& v)
    : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z) { }
    // vector_proxy(const vector_proxy& proxy);
    // ~vector_proxy();

    void eval(scalar_units<m, d, t>& x,
              scalar_units<m, d, t>& y,
              scalar_units<m, d, t>& z) const
    { x = m_x; y = m_y; z = m_z; }

  private:
    scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, op> > m_x;
    scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, op> > m_y;
    scalar_proxy<m, d, t, scalar_unary_proxy<mpfr_t, op> > m_z;

    vector_proxy& operator=(const vector_proxy&);
  };

  // Binary vector_proxy's

  template<int m, int d, int t, typename T, typename U, typename op>
  class vector_proxy<m, d, t, scalar_binary_proxy<T, U, op> >
  {
    template<int m2, int d2, int t2, typename op2> friend class vector_proxy;

  public:
    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_proxy<m1, d1, t1, T>& lhs,
                 const vector_proxy<m2, d2, t2, U>& rhs)
    : m_x(lhs.m_x, rhs.m_x), m_y(lhs.m_y, rhs.m_y), m_z(lhs.m_z, rhs.m_z) { }

    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_proxy<m1, d1, t1, T>& lhs,
                 const scalar_proxy<m2, d2, t2, U>& rhs)
    : m_x(lhs.m_x, rhs), m_y(lhs.m_y, rhs), m_z(lhs.m_z, rhs) { }

    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const scalar_proxy<m1, d1, t1, T>& lhs,
                 const vector_proxy<m2, d2, t2, U>& rhs)
    : m_x(lhs, rhs.m_x), m_y(lhs, rhs.m_y), m_z(lhs, rhs.m_z) { }

    // vector_proxy(const vector_proxy& proxy);
    // ~vector_proxy();

    void eval(scalar_units<m, d, t>& x,
              scalar_units<m, d, t>& y,
              scalar_units<m, d, t>& z) const
    { x = m_x; y = m_y; z = m_z; }

  private:
    scalar_proxy<m, d, t, scalar_binary_proxy<T, U, op> > m_x;
    scalar_proxy<m, d, t, scalar_binary_proxy<T, U, op> > m_y;
    scalar_proxy<m, d, t, scalar_binary_proxy<T, U, op> > m_z;

    vector_proxy& operator=(const vector_proxy&);
  };

  template<int m, int d, int t, typename T, typename op>
  class vector_proxy<m, d, t, scalar_binary_proxy<T, mpfr_t, op> >
  {
    template<int m2, int d2, int t2, typename op2> friend class vector_proxy;

  public:
    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_proxy<m1, d1, t1, T>& lhs,
                 const vector_units<m2, d2, t2>& rhs)
    : m_x(lhs.m_x, rhs.m_x), m_y(lhs.m_y, rhs.m_y), m_z(lhs.m_z, rhs.m_z) { }

    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_proxy<m1, d1, t1, T>& lhs,
                 const scalar_units<m2, d2, t2>& rhs)
    : m_x(lhs.m_x, rhs), m_y(lhs.m_y, rhs), m_z(lhs.m_z, rhs) { }

    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const scalar_proxy<m1, d1, t1, T>& lhs,
                 const vector_units<m2, d2, t2>& rhs)
    : m_x(lhs, rhs.m_x), m_y(lhs, rhs.m_y), m_z(lhs, rhs.m_z) { }

    // vector_proxy(const vector_proxy& proxy);
    // ~vector_proxy();

    void eval(scalar_units<m, d, t>& x,
              scalar_units<m, d, t>& y,
              scalar_units<m, d, t>& z) const
    { x = m_x; y = m_y; z = m_z; }

  private:
    scalar_proxy<m, d, t, scalar_binary_proxy<T, mpfr_t, op> > m_x;
    scalar_proxy<m, d, t, scalar_binary_proxy<T, mpfr_t, op> > m_y;
    scalar_proxy<m, d, t, scalar_binary_proxy<T, mpfr_t, op> > m_z;

    vector_proxy& operator=(const vector_proxy&);
  };

  template<int m, int d, int t, typename T, typename op>
  class vector_proxy<m, d, t, scalar_binary_proxy<mpfr_t, T, op> >
  {
    template<int m2, int d2, int t2, typename op2> friend class vector_proxy;

  public:
    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_units<m1, d1, t1>& lhs,
                 const vector_proxy<m2, d2, t2, T>& rhs)
    : m_x(lhs.m_x, rhs.m_x), m_y(lhs.m_y, rhs.m_y), m_z(lhs.m_z, rhs.m_z) { }

    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_units<m1, d1, t1>& lhs,
                 const scalar_proxy<m2, d2, t2, T>& rhs)
    : m_x(lhs.m_x, rhs), m_y(lhs.m_y, rhs), m_z(lhs.m_z, rhs) { }

    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const scalar_units<m1, d1, t1>& lhs,
                 const vector_proxy<m2, d2, t2, T>& rhs)
    : m_x(lhs, rhs.m_x), m_y(lhs, rhs.m_y), m_z(lhs, rhs.m_z) { }

    // vector_proxy(const vector_proxy& proxy);
    // ~vector_proxy();

    void eval(scalar_units<m, d, t>& x,
              scalar_units<m, d, t>& y,
              scalar_units<m, d, t>& z) const
    { x = m_x; y = m_y; z = m_z; }

  private:
    scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, T, op> > m_x;
    scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, T, op> > m_y;
    scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, T, op> > m_z;

    vector_proxy& operator=(const vector_proxy&);
  };

  template<int m, int d, int t, typename op>
  class vector_proxy<m, d, t, scalar_binary_proxy<mpfr_t, mpfr_t, op> >
  {
    template<int m2, int d2, int t2, typename op2> friend class vector_proxy;

  public:
    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_units<m1, d1, t1>& lhs,
                 const vector_units<m2, d2, t2>& rhs)
    : m_x(lhs.m_x, rhs.m_x), m_y(lhs.m_y, rhs.m_y), m_z(lhs.m_z, rhs.m_z) { }

    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_units<m1, d1, t1>& lhs,
                 const scalar_units<m2, d2, t2>& rhs)
    : m_x(lhs.m_x, rhs), m_y(lhs.m_y, rhs), m_z(lhs.m_z, rhs) { }

    template<int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const scalar_units<m1, d1, t1>& lhs,
                 const vector_units<m2, d2, t2>& rhs)
    : m_x(lhs, rhs.m_x), m_y(lhs, rhs.m_y), m_z(lhs, rhs.m_z) { }

    // vector_proxy(const vector_proxy& proxy);
    // ~vector_proxy();

    void eval(scalar_units<m, d, t>& x,
              scalar_units<m, d, t>& y,
              scalar_units<m, d, t>& z) const
    { x = m_x; y = m_y; z = m_z; }

  private:
    scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, mpfr_t, op> > m_x;
    scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, mpfr_t, op> > m_y;
    scalar_proxy<m, d, t, scalar_binary_proxy<mpfr_t, mpfr_t, op> > m_z;

    vector_proxy& operator=(const vector_proxy&);
  };
}

#endif // CAROM_VECTOR_PROXY_HPP
