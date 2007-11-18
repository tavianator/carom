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

#ifndef CAROM_SCALAR_SS_PROXY_HPP
#define CAROM_SCALAR_SS_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  // A unary proxy returning a scalar, and taking a scalar
  template <typename T, typename op>
  class ss_proxy
  {
  public:
    ss_proxy(const T& n) : m_n(n) { }
    // ss_proxy(const ss_proxy&);
    // ~ss_proxy();

    void eval(mpfr_t store) const {
      mpfr_t temp;
      mpfr_init(temp);
      m_n.eval(temp);
      op::eval(store, temp);
      mpfr_clear(temp);
    }

  private:
    T m_n;

    ss_proxy& operator=(const ss_proxy&);
  };

  template <typename op>
  class ss_proxy<mpfr_t, op>
  {
  public:
    ss_proxy(mpfr_t n) : m_n(n) { }
    // ss_proxy(const ss_proxy&);
    // ~ss_proxy();

    void eval(mpfr_t store) const {
      op::eval(store, m_n);
    }

  private:
    mpfr_ptr m_n; // mpfr_t can't be used in initializer lists

    ss_proxy& operator=(const ss_proxy&);
  };
}

#endif // CAROM_SCALAR_SS_PROXY_HPP
