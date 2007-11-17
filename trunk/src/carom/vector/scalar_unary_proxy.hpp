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

#ifndef CAROM_VECTOR_SCALAR_UNARY_PROXY_HPP
#define CAROM_VECTOR_SCALAR_UNARY_PROXY_HPP

namespace carom
{
  template <typename T, typename op>
  class scalar_vector_unary_proxy
  {
  public:
    scalar_vector_unary_proxy(const T& n) : m_n(n) { }
    // scalar_vector_unary_proxy(const scalar_vector_unary_proxy& proxy);
    // ~scalar_vector_unary_proxy();

    void eval(mpfr_t store) const {
      mpfr_t x, y, z;
      mpfr_init(x);
      mpfr_init(y);
      mpfr_init(z);
      op::eval(store, x, y, z);
      mpfr_clear(x);
      mpfr_clear(y);
      mpfr_clear(z);
    }

  private:
    T m_n;

    scalar_vector_unary_proxy& operator=(const scalar_vector_unary_proxy&);
  };

  template <typename op>
  class scalar_vector_unary_proxy<mpfr_t, op>
  {
  public:
    scalar_vector_unary_proxy(mpfr_t x, mpfr_t y, mpfr_t z)
      : m_x(x), m_y(y), m_z(z) { }
    // scalar_vector_unary_proxy(const scalar_vector_unary_proxy& proxy);
    // ~scalar_vector_unary_proxy();

    void eval(mpfr_t store) const {
      op::eval(store, m_x, m_y, m_z);
    }

  private:
    mpfr_ptr m_x, m_y, m_z;

    scalar_vector_unary_proxy& operator=(const scalar_vector_unary_proxy&);
  };
}

#endif // CAROM_VECTOR_SCALAR_UNARY_PROXY_HPP
