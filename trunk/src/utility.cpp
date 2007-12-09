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

namespace carom
{
  scalar_mass mass(const body& b) {
    scalar_mass m = 0;
    for (body::const_iterator i = b.begin(); i != b.end(); ++i) {
      m += i->m();
    }
    return m;
  }

  vector_displacement center_of_mass(const body& b) {
    vector_units<1, 1, 0> r = 0;
    for (body::const_iterator i = b.begin(); i != b.end(); ++i) {
      r += i->m()*i->s();
    }
    return r/mass(b);
  }

  vector_velocity velocity(const body& b) {
    return momentum(b)/mass(b);
  }

  vector_momentum momentum(const body& b) {
    vector_momentum p = 0;
    for (body::const_iterator i = b.begin(); i != b.end(); ++i) {
      p += i->p();
    }
    return p;
  }

  vector_acceleration acceleration(const body& b) {
    return force(b)/mass(b);
  }

  vector_force force(const body& b) {
    vector_force F = 0;
    for (body::const_iterator i = b.begin(); i != b.end(); ++i) {
      F += i->F();
    }
    return F;
  }

  scalar_moment_of_inertia
  moment_of_inertia(const body& b, const vector_displacement& o,
                    const vector& axis) {
    scalar_moment_of_inertia I = 0;
    for (body::const_iterator i = b.begin(); i != b.end(); ++i) {
      scalar_distance r = norm(i->s() - proj(axis, i->s() - o));
      I += i->m()*r*r;
    }
    return I;
  }

  vector_angular_velocity
  angular_velocity(const body& b, const vector_displacement& o,
                   const vector& axis) {
    return angular_momentum(b, o)/moment_of_inertia(b, o, axis);
  }

  vector_angular_momentum
  angular_momentum(const body& b, const vector_displacement& o) {
    vector_angular_momentum L = 0;
    for (body::const_iterator i = b.begin(); i != b.end(); ++i) {
      L += cross(i->s() - o, i->p());
    }
    return L;
  }

  vector_angular_acceleration
  angular_acceleration(const body& b, const vector_displacement& o,
                       const vector& axis) {
    return torque(b, o)/moment_of_inertia(b, o, axis);
  }

  vector_torque torque(const body& b, const vector_displacement& o) {
    vector_torque T = 0;
    for (body::const_iterator i = b.begin(); i != b.end(); ++i) {
      T += cross(i->s() - o, i->F());
    }
    return T;
  }

  void apply_forces(body& b) {
    for (body::iterator i = b.begin(); i != b.end(); ++i) {
      i->apply_forces();
    }
  }

  void clear_forces(body& b) {
    for (body::iterator i = b.begin(); i != b.end(); ++i) {
      i->clear_forces();
    }
  }
}
