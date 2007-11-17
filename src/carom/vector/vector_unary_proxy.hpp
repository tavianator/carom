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

#ifndef CAROM_VECTOR_VECTOR_UNARY_PROXY_HPP
#define CAROM_VECTOR_VECTOR_UNARY_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  template <typename T, typename op>
  class vector_vector_unary_proxy
  {
  public:
    vector_vector_unary_proxy(const T& n) : m_n(n) { }
    // vector_vector_unary_proxy(const vector_vector_unary_proxy& proxy);
    // ~vector_vector_unary_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      mpfr_t x, y, z;
      mpfr_init(x);
      mpfr_init(y);
      mpfr_init(z);
      m_n.eval(x, y, z);
      op::eval(store_x, store_y, store_z, x, y, z);
      mpfr_clear(x);
      mpfr_clear(y);
      mpfr_clear(z);
    }

  private:
    T m_n;

    vector_vector_unary_proxy& operator=(const vector_vector_unary_proxy&);
  };

  template <typename op>
  class vector_vector_unary_proxy<mpfr_t, op>
  {
  public:
    vector_vector_unary_proxy(mpfr_t x, mpfr_t y, mpfr_t z)
      : m_x(x), m_y(y), m_z(z) { }
    // vector_vector_unary_proxy(const vector_vector_unary_proxy& proxy);
    // ~vector_vector_unary_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const
    { op::eval(store_x, store_y, store_z, m_x, m_y, m_z); }

  private:
    mpfr_ptr m_x;
    mpfr_ptr m_y;
    mpfr_ptr m_z;

    vector_vector_unary_proxy& operator=(const vector_vector_unary_proxy&);
  };
}

#endif // CAROM_VECTOR_VECTOR_UNARY_PROXY_HPP
