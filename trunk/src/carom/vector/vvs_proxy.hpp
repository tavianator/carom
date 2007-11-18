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

#ifndef CAROM_VECTOR_VVS_PROXY_HPP
#define CAROM_VECTOR_VVS_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  // A binary proxy returning a vector, and taking a vector and a scalar
  template <typename T, typename U, typename op>
  class vvs_proxy
  {
  public:
    vvs_proxy(const T& lhs, const U& rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // vvs_proxy(const vvs_proxy&);
    // ~vvs_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      mpfr_t lhs_x, lhs_y, lhs_z;
      mpfr_t rhs;
      mpfr_init(lhs_x); mpfr_init(lhs_y); mpfr_init(lhs_z);
      mpfr_init(rhs);
      m_lhs.eval(lhs_x, lhs_y, lhs_z);
      m_rhs.eval(rhs);
      op::eval(store_x, store_y, store_z, lhs_x, lhs_y, lhs_z, rhs);
      mpfr_clear(lhs_x); mpfr_clear(lhs_y); mpfr_clear(lhs_z);
      mpfr_clear(rhs);
    }

  private:
    T m_lhs;
    U m_rhs;

    vvs_proxy& operator=(const vvs_proxy&);
  };

  template <typename T, typename op>
  class vvs_proxy<T, mpfr_t, op>
  {
  public:
    vvs_proxy(const T& lhs, mpfr_t rhs)
      : m_lhs(lhs), m_rhs(rhs) { }
    // vvs_proxy(const vvs_proxy&);
    // ~vvs_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      mpfr_t lhs_x, lhs_y, lhs_z;
      mpfr_init(lhs_x); mpfr_init(lhs_y); mpfr_init(lhs_z);
      m_lhs.eval(lhs_x, lhs_y, lhs_z);
      op::eval(store_x, store_y, store_z, lhs_x, lhs_y, lhs_z, m_rhs);
      mpfr_clear(lhs_x); mpfr_clear(lhs_y); mpfr_clear(lhs_z);
    }

  private:
    T        m_lhs;
    mpfr_ptr m_rhs;

    vvs_proxy& operator=(const vvs_proxy&);
  };

  template <typename T, typename op>
  class vvs_proxy<mpfr_t, T, op>
  {
  public:
    vvs_proxy(mpfr_t lhs_x, mpfr_t lhs_y, mpfr_t lhs_z, const T& rhs)
      : m_lhs_x(lhs_x), m_lhs_y(lhs_y), m_lhs_z(lhs_z), m_rhs(rhs) { }
    // vvs_proxy(const vvs_proxy&);
    // ~vvs_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      mpfr_t rhs;
      mpfr_init(rhs);
      m_rhs.eval(rhs);
      op::eval(store_x, store_y, store_z, m_lhs_x, m_lhs_y, m_lhs_z, rhs);
      mpfr_clear(rhs);
    }

  private:
    mpfr_ptr m_lhs_x, m_lhs_y, m_lhs_z;
    T        m_rhs;

    vvs_proxy& operator=(const vvs_proxy&);
  };

  template <typename op>
  class vvs_proxy<mpfr_t, mpfr_t, op>
  {
  public:
    vvs_proxy(mpfr_t lhs_x, mpfr_t lhs_y, mpfr_t lhs_z, mpfr_t rhs)
      : m_lhs_x(lhs_x), m_lhs_y(lhs_y), m_lhs_z(lhs_z), m_rhs(rhs) { }
    // vvs_proxy(const vvs_proxy&);
    // ~vvs_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const {
      op::eval(store_x, store_y, store_z, m_lhs_x, m_lhs_y, m_lhs_z, m_rhs);
    }

  private:
    mpfr_ptr m_lhs_x, m_lhs_y, m_lhs_z;
    mpfr_ptr m_rhs;

    vvs_proxy& operator=(const vvs_proxy&);
  };
}

#endif // CAROM_VECTOR_VVS_PROXY_HPP
