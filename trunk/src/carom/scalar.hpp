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

#include <mpfr.h>
#include <algorithm>

namespace carom
{
  template <int m, int d, int t>
  class scalar_units
  {
  public:
    scalar_units() { mpfr_init(m_fp); }

    template <typename T>
    scalar_units(T n) {
      mpfr_init(m_fp);
      mpfr_from(m_fp, n);
    }

    scalar_units(const scalar_units<m, d, t>& n) {
      mpfr_init_set(m_fp, n.m_fp, GMP_RNDN);
    }

#if 0
    scalar_units(scalar_units<m, d, t>&& n) : m_fp(n.m_fp) { n.m_fp = 0; }
#endif

    ~scalar_units() {
      if (m_fp != 0) {
        mpfr_clear(m_fp);
      }
    }

    template <typename T>
    scalar_units& operator=(T n) { mpfr_from(m_fp, n); }

    scalar_units& operator=(const scalar_units<m, d, t>& n) {
      mpfr_set(m_fp, n.m_fp, GMP_RNDN);
    }

#if 0
    scalar_units& operator=(scalar_units<m, d, t>&& n) {
      std::swap(m_fp, n.m_fp); // Shallow copy
    }
#endif

  private:
    mpfr_ptr m_fp;
  };

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

#endif // CAROM_SCALAR_HPP
