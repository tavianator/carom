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
  constant_force::constant_force(const vector_force& F) : m_F(F) { }
  constant_force::~constant_force() { }
  vector_force constant_force::force(const particle& x) const { return m_F; }

  centripetal_force::centripetal_force(const vector_displacement& o)
    : m_o(o) { }
  centripetal_force::~centripetal_force() { }

  vector_force centripetal_force::force(const particle& x) const {
    // F = m*v^2/r;
    vector_displacement r = x.s() - m_o;
    return -x.m()*norm(x.v())*norm(x.v())*normalized(r)/norm(r);
  }

  scalar_gravitational_constant gravitational_force::s_G("6.693e-11");

  gravitational_force::gravitational_force(const particle& x) : m_x(&x) { }
  gravitational_force::~gravitational_force() { }

  scalar_gravitational_constant gravitational_force::G() { return s_G; }

  void gravitational_force::G(const scalar_gravitational_constant& G) {
    s_G = G;
  }

  vector_force gravitational_force::force(const particle& x) const {
    // F = G*m1*m2/r^2
    vector_displacement r = x.s() - m_x->s();
    return -G()*x.m()*m_x->m()*normalized(r)/(norm(r)*norm(r));
  }

  spring_force::spring_force(const vector_displacement& o,
                             const scalar_distance& l,
                             const scalar_spring_constant& k)
    : m_o(o), m_l(l), m_k(k) { }
  spring_force::~spring_force() { }

  vector_force spring_force::force(const particle& x) const {
    // F = -k*s
    vector_displacement r = x.s() - m_o;
    if (r != 0) {
      return -m_k*normalized(r)*(norm(r) - m_l);
    } else {
      return 0;
    }
  }
}
