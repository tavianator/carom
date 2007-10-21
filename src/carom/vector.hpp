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

#ifndef CAROM_VECTOR_HPP
#define CAROM_VECTOR_HPP

namespace carom
{
  // See the comments in scalar.hpp for an outline of how unit-correctness
  // works; it is almost the same here.

  template<int m, int d, int t>              class vector_units;
  template<int m, int d, int t, typename op> class vector_proxy;

  // Convienent typedefs
  typedef vector_units<0, 0, 0>  vector;
  typedef vector_units<0, 1, 0>  vector_displacement;
  typedef vector_units<0, 1, -1> vector_velocity;
  typedef vector_units<1, 1, -1> vector_momentum;
  typedef vector_units<0, 1, -2> vector_acceleration;
  typedef vector_units<1, 1, -2> vector_force;
}

#include <carom/vector/proxy.hpp>
#include <carom/vector/vector.hpp>
#include <carom/vector/ops.hpp>

#endif // CAROM_VECTOR_HPP
