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

#ifndef CAROM_VECTOR_SV_PROXY_HPP
#define CAROM_VECTOR_SV_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  // A unary proxy returning a scalar, and taking a vector
  template <typename T, typename op>
  class sv_proxy
  {
  public:
    sv_proxy(const T& n) : m_n(n) { }
    // sv_proxy(const sv_proxy&);
    // ~sv_proxy();

    void eval(mpfr_t store) const {
      mpfr_t x, y, z;
      mpfr_init(x); mpfr_init(y); mpfr_init(z);
      m_n.eval(x, y, z);
      op::eval(store, x, y, z);
      mpfr_clear(x); mpfr_clear(y); mpfr_clear(z);
    }

  private:
    T m_n;

    sv_proxy& operator=(const sv_proxy&);
  };

  template <typename op>
  class sv_proxy<mpfr_t, op>
  {
  public:
    sv_proxy(mpfr_t x, mpfr_t y, mpfr_t z) : m_x(x), m_y(y), m_z(z) { }
    // sv_proxy(const sv_proxy&);
    // ~sv_proxy();

    void eval(mpfr_t store) const {
      op::eval(store, m_x, m_y, m_z);
    }

  private:
    mpfr_ptr m_x, m_y, m_z;

    sv_proxy& operator=(const sv_proxy&);
  };
}

#endif // CAROM_VECTOR_SV_PROXY_HPP
