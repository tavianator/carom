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

#ifndef CAROM_VECTOR_VECTOR_PROXY_HPP
#define CAROM_VECTOR_VECTOR_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  template <int m, int d, int t, typename T, typename op>
  class vector_proxy<m, d, t, vv_proxy<T, op> > : public vv_proxy<T, op>
  {
  public:
    vector_proxy(const T& n) : vv_proxy<T, op>(n) { }

    template <int m2, int d2, int t2>
    vector_proxy(const vector_units<m2, d2, t2>& n)
      : vv_proxy<T, op>(n.m_x, n.m_y, n.m_z) { }
  };

  template <int m, int d, int t, typename T, typename U, typename op>
  class vector_proxy<m, d, t, vvv_proxy<T, U, op> > : public vvv_proxy<T, U, op>
  {
  public:
    vector_proxy(const T& lhs, const U& rhs)
      : vvv_proxy<T, U, op>(lhs, rhs) { }

    template <int m2, int d2, int t2>
    vector_proxy(const T& lhs, const vector_units<m2, d2, t2>& rhs)
      : vvv_proxy<T, U, op>(lhs, rhs.m_x, rhs.m_y, rhs.m_z) { }

    template <int m2, int d2, int t2>
    vector_proxy(const vector_units<m2, d2, t2>& lhs, const U& rhs)
      : vvv_proxy<T, U, op>(lhs.m_x, lhs.m_y, lhs.m_z, rhs) { }

    template <int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_units<m1, d2, t2>& lhs,
		 const vector_units<m2, d2, t2>& rhs)
      : vvv_proxy<T, U, op>(lhs.m_x, lhs.m_y, lhs.m_z,
                            rhs.m_x, rhs.m_y, rhs.m_z) { }
  };

  template <int m, int d, int t, typename T, typename U, typename op>
  class vector_proxy<m, d, t, vvs_proxy<T, U, op> > : public vvs_proxy<T, U, op>
  {
  public:
    vector_proxy(const T& lhs, const U& rhs)
      : vvs_proxy<T, U, op>(lhs, rhs) { }

    template <int m2, int d2, int t2>
    vector_proxy(const T& lhs, const scalar_units<m2, d2, t2>& rhs)
      : vvs_proxy<T, U, op>(lhs, rhs.fp) { }

    template <int m2, int d2, int t2>
    vector_proxy(const vector_units<m2, d2, t2>& lhs, const U& rhs)
      : vvs_proxy<T, U, op>(lhs.m_x, lhs.m_y, lhs.m_z, rhs) { }

    template <int m1, int m2, int d1, int d2, int t1, int t2>
    vector_proxy(const vector_units<m1, d2, t2>& lhs,
		 const scalar_units<m2, d2, t2>& rhs)
      : vvs_proxy<T, U, op>(lhs.m_x, lhs.m_y, lhs.m_z, rhs.m_fp) { }
  };
}

#endif // CAROM_VECTOR_VECTOR_PROXY_HPP
