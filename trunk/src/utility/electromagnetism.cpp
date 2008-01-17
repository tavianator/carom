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
  // This order is important!
  scalar_units<1, 1, 0>  electromagnetic_force::s_u(
    convert<1, 1, 0>(4*pi()*scalar("1e-7"))
                                                    );

  scalar_units<-1, -3, 2> electromagnetic_force::s_e(
    1/s_u/scalar_speed("299792458")/scalar_speed("299792458")
                                                     );

  electromagnetic_force::~electromagnetic_force() {
  }

  vector_force electromagnetic_force::force(const particle& x) {
    // F = q*(E + v X B)

    const charge* q = dynamic_cast<const charge*>(&x);

    if (q != 0) {
      vector_displacement r = m_x->s() - x.s();
      vector_electric_field E =
        m_q->q()*normalized(r)/(4*pi()*e()*norm(r)*norm(r));
      vector_magnetic_field B =
        u()*m_q->q()*cross(m_x->v(), normalized(r))/(4*pi()*norm(r)*norm(r));
      return q->q()*(E + cross(x.v(), B));
    } else {
      return 0;
    }
  }

  electric_force::~electric_force() {
  }

  vector_force electric_force::force(const particle& x) {
    // F = q*E

    const charge* q = dynamic_cast<const charge*>(&x);

    if (q != 0) {
      return q->q()*m_E;
    } else {
      return 0;
    }
  }

  magnetic_force::~magnetic_force() {
  }

  vector_force magnetic_force::force(const particle& x) {
    // F = q*(v X B)

    const charge* q = dynamic_cast<const charge*>(&x);

    if (q != 0) {
      return q->q()*cross(x.v(), m_B);
    } else {
      return 0;
    }
  }

}
