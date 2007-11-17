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

#ifndef CAROM_VECTOR_VECTOR_NULLARY_PROXY_HPP
#define CAROM_VECTOR_VECTOR_NULLARY_PROXY_HPP

namespace carom
{
  template <typename op>
  class vector_nullary_proxy
  {
  public:
    // vector_nullary_proxy();
    // vector_nullary_proxy(const vector_nullary_proxy& proxy);
    // ~vector_nullary_proxy();

    void eval(mpfr_t store_x, mpfr_t store_y, mpfr_t store_z) const
    { op::eval(store_x, store_y, store_z); }

  private:
    vector_nullary_proxy& operator=(const vector_nullary_proxy&);
  };
}

#endif // CAROM_VECTOR_VECTOR_NULLARY_PROXY_HPP
