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
  // Elastic collision response
  void collision(body& b1, body& b2) {
    impenetrable* ib1 = dynamic_cast<impenetrable*>(&b1);
    impenetrable* ib2 = dynamic_cast<impenetrable*>(&b2);

    if (ib1 != 0) {
      vector_displacement o1 = ib1->surface().center();

      for (body::iterator i = b2.begin(); i != b2.end(); ++i) {
        mesh::iterator j = ib1->surface().inside(o1, i->s());

        if (j != ib1->surface().end() && dot(i->s() - o1, i->v()) > 0) {
          scalar_mass     m1 = ib1->mass(*j);
          scalar_mass     m2 = i->m();
          vector_momentum p1 = m1*j->v();
          vector_momentum p2 = i->p();

          vector_momentum dp1 = 2*(m1*p2 - m2*p1)/(m1 + m2);
          vector_momentum dp2 = 2*(m2*p2 - m1*p2)/(m1 + m2);

          ib1->collision(*j, dp1);
          i->p(i->p() + dp2);
        }
      }
    }

    if (ib2 != 0) {
      vector_displacement o2 = ib2->surface().center();
 
      for (body::iterator i = b1.begin(); i != b1.end(); ++i) {
        mesh::iterator j = ib2->surface().inside(o2, i->s());

        if (j != ib2->surface().end() && dot(i->s() - o2, i->v()) > 0) {
          scalar_mass     m1 = i->m();
          scalar_mass     m2 = ib2->mass(*j);
          vector_momentum p1 = i->p();
          vector_momentum p2 = m2*j->v();

          vector_momentum dp1 = 2*(m1*p2 - m2*p1)/(m1 + m2);
          vector_momentum dp2 = 2*(m2*p2 - m1*p2)/(m1 + m2);

          i->p(i->p() + dp1);
          ib2->collision(*j, dp2);
        }
      }
    }
  }
}
