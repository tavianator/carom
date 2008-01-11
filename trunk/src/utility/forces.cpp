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
  vector_force constant_force::force(const particle& x) const {
    return m_F;
  }

  vector_force centripetal_force::force(const particle& x) const {
    // F = m*v^2/r;
    vector_displacement r = x.s() - m_o;
    return -x.m()*norm(x.v())*norm(x.v())*normalized(r)/norm(r);
  }

  scalar_units<-1, 3, -2> gravitational_force::s_G("6.6742e-11");

  vector_force gravitational_force::force(const particle& x) const {
    // F = G*m1*m2/r^2
    vector_displacement r = m_i->s() - x.s();
    return G()*x.m()*m_i->m()*normalized(r)/(norm(r)*norm(r));
  }
}
