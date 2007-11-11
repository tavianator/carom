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
#include <string>

namespace carom
{
  template <int m, int d, int t>
  class vector_units
  {
    template <int m2, int d2, int t2, typename op> friend class vector_proxy;

    // Intentionally non-template friend functions; declared in situ. Use
    // -Wno-non-template-friends to suppress g++'s warning.
    friend bool operator==(const vector_units<m, d, t>& lhs,
                           const vector_units<m, d, t>& rhs)
    { return lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y && lhs.m_z == rhs.m_z; }
    friend bool operator!=(const vector_units<m, d, t>& lhs,
                           const vector_units<m, d, t>& rhs)
    { return lhs.m_x != rhs.m_x || lhs.m_y != rhs.m_y || lhs.m_z != rhs.m_z; }

  public:
    vector_units() { }
    vector_units(void*) : m_x(0), m_y(0), m_z(0) { }
    vector_units(const scalar_units<m, d, t>& x,
                 const scalar_units<m, d, t>& y,
                 const scalar_units<m, d, t>& z) : m_x(x), m_y(y), m_z(z) { }
    template <typename op> vector_units(const vector_proxy<m, d, t, op>& proxy)
    { proxy.eval(m_x, m_y, m_z); }
    // vector_units(const vector_units<m, d, t>& v);
    // ~vector_units();

    template <typename op>
    vector_units& operator=(const vector_proxy<m, d, t, op>& proxy)
    { proxy.eval(m_x, m_y, m_z); return *this; }

    // vector_units& operator=(const vector_units<m, d, t>& rhs);

    vector_units& operator+=(const vector_units<m, d, t>& rhs)
    { m_x += rhs.m_x; m_y += rhs.m_y; m_z += rhs.m_z; return *this; }
    vector_units& operator-=(const vector_units<m, d, t>& rhs)
    { m_x -= rhs.m_x; m_y -= rhs.m_y; m_z -= rhs.m_z; return *this; }
    vector_units& operator*=(const scalar_units<0, 0, 0>& rhs)
    { m_x *= rhs; m_y *= rhs; m_z *= rhs; return *this; }
    vector_units& operator/=(const scalar_units<0, 0, 0>& rhs)
    { m_x /= rhs; m_y /= rhs; m_z /= rhs; return *this; }

    const scalar_units<m, d, t> x() { return m_x; }
    const scalar_units<m, d, t> y() { return m_y; }
    const scalar_units<m, d, t> z() { return m_z; }

  private:
    scalar_units<m, d, t> m_x;
    scalar_units<m, d, t> m_y;
    scalar_units<m, d, t> m_z;
  };
}

#endif // CAROM_VECTOR_VECTOR_HPP
