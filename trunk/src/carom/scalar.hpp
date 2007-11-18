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

#ifndef CAROM_SCALAR_HPP
#define CAROM_SCALAR_HPP

namespace carom
{
  // Foreward declarations
  template <int m, int d, int t, typename op> class scalar_proxy;
  template <int m, int d, int t>              class scalar_units;

  // Convenient typedefs
  typedef scalar_units<0, 0, 0>  scalar;
  typedef scalar_units<0, 0, 0>  scalar_angle;
  typedef scalar_units<1, 0, 0>  scalar_mass;
  typedef scalar_units<0, 1, 0>  scalar_distance;
  typedef scalar_units<0, 0, 1>  scalar_time;
  typedef scalar_units<0, 1, -1> scalar_speed;
  typedef scalar_units<1, 1, -1> scalar_momentum;
  typedef scalar_units<0, 1, -2> scalar_acceleration;
  typedef scalar_units<1, 1, -2> scalar_force;
  typedef scalar_units<1, 2, -2> scalar_energy;
  typedef scalar_units<1, 2, -3> scalar_power;
}

#include <carom/scalar/s_proxy.hpp>
#include <carom/scalar/ss_proxy.hpp>
#include <carom/scalar/sss_proxy.hpp>
#include <carom/scalar/proxy.hpp>
#include <carom/scalar/scalar.hpp>
#include <carom/scalar/ops.hpp>

#endif // CAROM_SCALAR_HPP
