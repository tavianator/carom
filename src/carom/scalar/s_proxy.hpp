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

#ifndef CAROM_SCALAR_S_PROXY_HPP
#define CAROM_SCALAR_S_PROXY_HPP

#include <mpfr.h>

namespace carom
{
  // Nullary proxy returning a scalar
  template <typename op>
  class s_proxy
  {
  public:
    // s_proxy();
    // s_proxy(const s_proxy& proxy);
    // ~s_proxy();

    void eval(mpfr_t store) const { op::eval(store); }

  private:
    s_proxy& operator=(const s_proxy&);
  };
}

#endif // CAROM_SCALAR_S_PROXY_HPP
