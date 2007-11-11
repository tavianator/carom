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

#ifndef CAROM_SCALAR_BINARY_PROXY_HPP
#define CAROM_SCALAR_BINARY_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  // Binary operator proxy class
  template <typename T, typename U, typename op>
  class scalar_binary_proxy
  {
  public:
    scalar_binary_proxy(const T& lhs, const U& rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // scalar_binary_proxy(const scalar_binary_proxy& proxy);
    // ~scalar_binary_proxy();

    void eval(mpfr_t store) const {
      mpfr_t temp1, temp2;
      mpfr_init(temp1);
      mpfr_init(temp2);
      m_lhs.eval(temp1);
      m_rhs.eval(temp2);
      op::eval(store, temp1, temp2);
      mpfr_clear(temp1);
      mpfr_clear(temp2);
    }

  private:
    T m_lhs;
    U m_rhs;

    scalar_binary_proxy& operator=(const scalar_binary_proxy&);
  };

  template <typename T, typename op>
  class scalar_binary_proxy<T, mpfr_t, op>
  {
  public:
    scalar_binary_proxy(const T& lhs, mpfr_t rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // scalar_binary_proxy(const scalar_binary_proxy& proxy);
    // ~scalar_binary_proxy();

    void eval(mpfr_t store) const {
      mpfr_t temp;
      mpfr_init(temp);
      m_lhs.eval(temp);
      op::eval(store, temp, m_rhs);
      mpfr_clear(temp);
    }

  private:
    T m_lhs;
    mpfr_ptr m_rhs;

    scalar_binary_proxy& operator=(const scalar_binary_proxy&);
  };

  template <typename T, typename op>
  class scalar_binary_proxy<mpfr_t, T, op>
  {
  public:
    scalar_binary_proxy(mpfr_t lhs, const T& rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // scalar_binary_proxy(const scalar_binary_proxy& proxy);
    // ~scalar_binary_proxy();

    void eval(mpfr_t store) const {
      mpfr_t temp;
      mpfr_init(temp);
      m_rhs.eval(temp);
      op::eval(store, m_lhs, temp);
      mpfr_clear(temp);
    }

  private:
    mpfr_ptr m_lhs;
    T m_rhs;

    scalar_binary_proxy& operator=(const scalar_binary_proxy&);
  };

  template <typename op>
  class scalar_binary_proxy<mpfr_t, mpfr_t, op>
  {
  public:
    scalar_binary_proxy(mpfr_t lhs, mpfr_t rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // scalar_binary_proxy(const scalar_binary_proxy& proxy);
    // ~scalar_binary_proxy();

    void eval(mpfr_t store) const { op::eval(store, m_lhs, m_rhs); }

  private:
    mpfr_ptr m_lhs;
    mpfr_ptr m_rhs;

    scalar_binary_proxy& operator=(const scalar_binary_proxy&);
  };
}

#endif // CAROM_SCALAR_BINARY_PROXY_HPP
