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

#ifndef CAROM_VECTOR_VVV_PROXY_HPP
#define CAROM_VECTOR_VVV_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  // A binary proxy returning a vector, and taking two vectors
  template <typename T, typename U, typename op>
  class vvv_proxy
  {
  public:
    vvv_proxy(const T& lhs, const U& rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // vvv_proxy(const vvv_proxy&);
    // ~vvv_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      mpfr_t lhs_x, lhs_y, lhs_z;
      mpfr_t rhs_x, rhs_y, rhs_z;
      mpfr_init(lhs_x); mpfr_init(lhs_y); mpfr_init(lhs_z);
      mpfr_init(rhs_x); mpfr_init(rhs_y); mpfr_init(rhs_z);
      m_lhs.eval(lhs_x, lhs_y, lhs_z);
      m_rhs.eval(rhs_x, lhs_y, lhs_z);
      op::eval(store_x, store_y, store_z,
               lhs_x,   lhs_y,   lhs_z,
               rhs_x,   rhs_y,   rhs_z);
      mpfr_clear(lhs_x); mpfr_clear(lhs_y); mpfr_clear(lhs_z);
      mpfr_clear(rhs_x); mpfr_clear(rhs_y); mpfr_clear(rhs_z);
    }

  private:
    T m_lhs;
    U m_rhs;

    vvv_proxy& operator=(const vvv_proxy&);
  };

  template <typename T, typename op>
  class vvv_proxy<T, mpfr_t, op>
  {
  public:
    vvv_proxy(const T& lhs, mpfr_t rhs_x, mpfr_t rhs_y, mpfr_t rhs_z)
      : m_lhs(lhs), m_rhs_x(rhs_x), m_rhs_y(rhs_y), m_rhs_z(rhs_z) { }
    // vvv_proxy(const vvv_proxy&);
    // ~vvv_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      mpfr_t lhs_x, lhs_y, lhs_z;
      mpfr_init(lhs_x); mpfr_init(lhs_y); mpfr_init(lhs_z);
      m_lhs.eval(lhs_x, lhs_y, lhs_z);
      op::eval(store_x, store_y, store_z,
               lhs_x,   lhs_y,   lhs_z,
               m_rhs_x, m_rhs_y, m_rhs_z); 
      mpfr_clear(lhs_x); mpfr_clear(lhs_y); mpfr_clear(lhs_z);
    }

  private:
    T        m_lhs;
    mpfr_ptr m_rhs_x, m_rhs_y, m_rhs_z;

    vvv_proxy& operator=(const vvv_proxy&);
  };

  template <typename T, typename op>
  class vvv_proxy<mpfr_t, T, op>
  {
  public:
    vvv_proxy(mpfr_t lhs_x, mpfr_t lhs_y, mpfr_t lhs_z, const T& rhs)
      : m_lhs_x(lhs_x), m_lhs_y(lhs_y), m_lhs_z(lhs_z), m_rhs(rhs) { }
    // vvv_proxy(const vvv_proxy&);
    // ~vvv_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      mpfr_t rhs_x, rhs_y, rhs_z;
      mpfr_init(rhs_x); mpfr_init(rhs_y); mpfr_init(rhs_z);
      m_rhs.eval(rhs_x, rhs_y, rhs_z);
      op::eval(store_x, store_y, store_z,
               m_lhs_x, m_lhs_y, m_lhs_z,
               rhs_x,   rhs_y,   rhs_z); 
      mpfr_clear(rhs_x); mpfr_clear(rhs_y); mpfr_clear(rhs_z);
    }

  private:
    mpfr_ptr m_lhs_x, m_lhs_y, m_lhs_z;
    T        m_rhs;

    vvv_proxy& operator=(const vvv_proxy&);
  };

  template <typename op>
  class vvv_proxy<mpfr_t, mpfr_t, op>
  {
  public:
    vvv_proxy(mpfr_t lhs_x, mpfr_t lhs_y, mpfr_t lhs_z,
              mpfr_t rhs_x, mpfr_t rhs_y, mpfr_t rhs_z)
      : m_lhs_x(lhs_x), m_lhs_y(lhs_y), m_lhs_z(lhs_z),
        m_rhs_x(rhs_x), m_rhs_y(rhs_y), m_rhs_z(rhs_z) { }
    // vvv_proxy(const vvv_proxy&);
    // ~vvv_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      op::eval(store_x, store_y, store_z,
               m_lhs_x, m_lhs_y, m_lhs_z,
               m_rhs_x, m_rhs_y, m_rhs_z);
    }

  private:
    mpfr_ptr m_lhs_x, m_lhs_y, m_lhs_z;
    mpfr_ptr m_rhs_x, m_rhs_y, m_rhs_z;

    vvv_proxy& operator=(const vvv_proxy&);
  };
}

#endif // CAROM_VECTOR_VVV_PROXY_HPP
