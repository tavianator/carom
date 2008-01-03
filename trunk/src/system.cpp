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

#include <carom.hpp>
#include <boost/utility.hpp> // For next()/prior()
#include <vector>

namespace carom
{
  void system::integrate_Euler(const scalar_time& t) {
    // Euler method. Simplest Runge-Kutta method. First order. Its tableau is:
    //
    //   0|
    //   -+-
    //    |1
    //
    // y[n + 1] = y[n] + t*f(y[n])

    std::vector<k_value> k_vec(size());

    std::vector<k_value>::iterator k;
    iterator i;

    for (i = begin(); i != end(); ++i) {
      i->begin_integration();
    }

    for (i = begin(), k = k_vec.begin(); i != end(); ++i, ++k) {
      *k = i->k();
    }

    for (i = begin(), k = k_vec.begin(); i != end(); ++i, ++k) {
      i->step(*k, t);
    }

    for (i = begin(); i != end(); ++i) {
      i->end_integration();
    }
  }

  void system::integrate_midpoint(const scalar_time& t) {
    // Midpoint method. Second order Runge-Kutta method. Requires only two
    // function evaluations per step. Its tableau is:
    //
    //   0  |
    //   1/2|1/2
    //   ---+-----
    //      |0   1
    //
    //    k1    = f(y[n])
    //    k2    = f(y[n] + (t/2)*k1)
    // y[n + 1] = y[n] + t*k2

    std::vector<k_value> k1_vec(size());
    std::vector<k_value> k2_vec(size());

    std::vector<k_value>::iterator k1, k2;
    iterator i;

    for (i = begin(); i != end(); ++i) {
      i->begin_integration();
    }

    for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
      *k1 = i->k();
    }

    for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
      i->step(*k1, t/2);
    }

    for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
      *k2 = i->k();
    }

    for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
      i->step(*k2, t);
    }

    for (i = begin(); i != end(); ++i) {
      i->end_integration();
    }
  }

  void system::integrate_RK4(const scalar_time& t) {
    // Classical, fourth order Runge-Kutta method. Requires four function
    // evaluations per step. Its tableau is:
    //
    //   0  |
    //   1/2|1/2
    //   1/2|0   1/2
    //   1  |0   0   1
    //   ---+---------------
    //      |1/6 1/3 1/3 1/6
    //
    //    k1    = f(y[n])
    //    k2    = f(y[n] + (t/2)*k1)
    //    k3    = f(y[n] + (t/2)*k2)
    //    k4    = f(y[n] + t*k3)
    // y[n + 1] = y[n] + t*(k1 + 2*k2 + 2*k3 + k4)/6

    std::vector<k_value> k1_vec(size());
    std::vector<k_value> k2_vec(size());
    std::vector<k_value> k3_vec(size());
    std::vector<k_value> k4_vec(size());

    std::vector<k_value>::iterator k1, k2, k3, k4;
    iterator i;

    for (i = begin(); i != end(); ++i) {
      i->begin_integration();
    }

    for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
      *k1 = i->k();
    }

    for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
      i->step(*k1, t/2);
    }

    for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
      *k2 = i->k();
    }

    for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
      i->step(*k2, t/2);
    }

    for (i = begin(), k3 = k3_vec.begin(); i != end(); ++i, ++k3) {
      *k3 = i->k();
    }

    for (i = begin(), k3 = k3_vec.begin(); i != end(); ++i, ++k3) {
      i->step(*k3, t);
    }

    for (i = begin(), k4 = k4_vec.begin(); i != end(); ++i, ++k4) {
      *k4 = i->k();
    }

    for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin(),
                      k3 = k3_vec.begin(), k4 = k4_vec.begin();
         i != end();
         ++i, ++k1, ++k2, ++k3, ++k4) {
      k_value k = (*k1 + 2*(*k2) + 2*(*k3) + *k4)/6;
      i->step(k, t);
    }

    for (i = begin(); i != end(); ++i) {
      i->end_integration();
    }
  }

  scalar_time system::integrate_DP(const scalar_time& t, scalar_time& elapsed) {
  }

  void system::collision() {
    for (iterator i = begin(); i != end(); ++i) {
      for (iterator j = boost::next(i); j != end(); ++j) {
        carom::collision(*i, *j); // collision() is hidden by system::collision
      }
    }
  }
}
