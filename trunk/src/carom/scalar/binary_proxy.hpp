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
  template <typename T, typename U, typename op>
  class scalar_binary_proxy
  {
  public:
    scalar_binary_proxy(const T& lhs, const U& rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // scalar_binary_proxy(const scalar_binary_proxy&);
    // ~scalar_binary_proxy();

    void eval(mpfr_t store) const {
      mpfr_t lhs;
      mpfr_t rhs;
      mpfr_init(lhs);
      mpfr_init(rhs);
      m_lhs.eval(lhs);
      m_rhs.eval(rhs);
      op::eval(store, lhs, rhs);
      mpfr_clear(lhs);
      mpfr_clear(rhs);
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
    // scalar_binary_proxy(const scalar_binary_proxy&);
    // ~scalar_binary_proxy();

    void eval(mpfr_t store) const {
      mpfr_t lhs;
      mpfr_init(lhs);
      m_lhs.eval(lhs);
      op::eval(store, lhs, m_rhs);
      mpfr_clear(lhs);
    }

  private:
    T        m_lhs;
    mpfr_ptr m_rhs;

    scalar_binary_proxy& operator=(const scalar_binary_proxy&);
  };

  template <typename T, typename op>
  class scalar_binary_proxy<mpfr_t, T, op>
  {
  public:
    scalar_binary_proxy(mpfr_t lhs, const T& rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // scalar_binary_proxy(const scalar_binary_proxy&);
    // ~scalar_binary_proxy();

    void eval(mpfr_t store) const {
      mpfr_t rhs;
      mpfr_init(rhs);
      m_rhs.eval(rhs);
      op::eval(store, m_lhs, rhs);
      mpfr_clear(rhs);
    }

  private:
    mpfr_ptr m_lhs;
    T        m_rhs;

    scalar_binary_proxy& operator=(const scalar_binary_proxy&);
  };

  template <typename op>
  class scalar_binary_proxy<mpfr_t, mpfr_t, op>
  {
  public:
    scalar_binary_proxy(mpfr_t lhs, mpfr_t rhs) : m_lhs(lhs), m_rhs(rhs) { }
    // scalar_binary_proxy(const scalar_binary_proxy&);
    // ~scalar_binary_proxy();

    void eval(mpfr_t store) const {
      op::eval(store, m_lhs, m_rhs);
    }

  private:
    mpfr_ptr m_lhs;
    mpfr_ptr m_rhs;

    scalar_binary_proxy& operator=(const scalar_binary_proxy&);
  };
}

#endif // CAROM_SCALAR_BINARY_PROXY_HPP
