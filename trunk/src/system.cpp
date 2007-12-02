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
#include <boost/next_prior.hpp> // For next()/prior()

namespace carom
{
  void system::integrate(const scalar_time& t) {
    for (iterator i = begin(); i != end(); ++i) {
      i->calculate_k1();
    }

    for (iterator i = begin(); i != end(); ++i) {
      i->apply_k1();
    }

    for (iterator i = begin(); i != end(); ++i) {
      i->calculate_k2();
    }

    for (iterator i = begin(); i != end(); ++i) {
      i->apply_k2();
    }

    for (iterator i = begin(); i != end(); ++i) {
      i->calculate_k3();
    }

    for (iterator i = begin(); i != end(); ++i) {
      i->apply_k3();
    }

    for (iterator i = begin(); i != end(); ++i) {
      i->calculate_k4();
    }

    for (iterator i = begin(); i != end(); ++i) {
      i->apply();
    }
  }

  void system::collision() {
    for (iterator i = begin(); i != end(); ++i) {
      for (iterator j = boost::next(i); j != end(); ++j) {
        carom::collision(*i, *j); // collision() is hidden by system::collision
      }
    }
  }
}
