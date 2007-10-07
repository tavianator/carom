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
  // Scalars, in The Carom Library, are wrappers for the mpfr library. This
  // enables clients of The Carom Library to get exactly the same results on
  // any hardware, unlike a simple use of `double', and to calculate results
  // to any numerical precision desired.
  // 
  // Carom Library scalars provide another useful feature: compile-time unit-
  // checking. This is accomplished through the use of three integer template
  // parameters, m, d, and t. m corresponds to the exponent of the mass unit,
  // d to the exponent of the distance unit, and t to the exponent of the time
  // unit. Only scalars with equal m, d, and t may be added or subtracted,
  // and when two scalars are multiplied or divided, their m's, d's, and t's are
  // added or subtracted, respectively, to form a scalar with different units.
  // 
  // The result of arethmetic operations on scalars is not simply another
  // scalar, however. In order to get rid of a reduntant copy-constructor call,
  // these operations return a proxy class, scalar_proxy. Only when a scalar
  // is constructed or assigned from a scalar_proxy does the necessary work
  // happen. The downsides to this method are:
  // 
  //   1. Calls to template functions like std::max with temporary expressions
  //      like a + b will not succeed, because the type of a + b is not
  //      scalar_units<m, d, t>.
  //   2. Accessing member functions doesn't work on temporary expressions like
  //      a + b.
  //   3. Without optimization turned on, scalars are grossly inneficient,
  //      because the proxy function calls wont be inlined.
  // 
  // In the first two cases, the problem can be solved by manually constructing
  // a scalar from a + b, as in scalar_units<m, d, t>(a + b).to<double>().
  // 
  // It is recomended to use the provided typedefs, like scalar, scalar_time,
  // and scalar_energy, rather than manually specifying scalar_units<m, d, t>,
  // whenever possible.

  // Foreward declarations
  template<int m, int d, int t>              class scalar_units;
  template<int m, int d, int t, typename op> class scalar_proxy;

  // Convienent typedefs
  typedef scalar_units<0, 0, 0>  scalar;
  typedef scalar_units<0, 0, 0>  scalar_angle;
  typedef scalar_units<1, 0, 0>  scalar_mass;
  typedef scalar_units<0, 1, 0>  scalar_distance;
  typedef scalar_units<0, 0, 1>  scalar_time;
  typedef scalar_units<0, 1, -1> scalar_speed;
  typedef scalar_units<1, 2, -2> scalar_energy;
  typedef scalar_units<1, 2, -3> scalar_power;
}

#include <carom/scalar/unary_proxy.hpp>
#include <carom/scalar/binary_proxy.hpp>
#include <carom/scalar/proxy.hpp>
#include <carom/scalar/scalar.hpp>
#include <carom/scalar/ops.hpp>

#endif // CAROM_SCALAR_HPP
