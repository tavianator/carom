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

namespace carom
{
  system::iterator system::insert(body* b) {
    return m_bodies.insert(m_bodies.end(), b);
  }

  void system::erase(system::iterator i) { m_bodies.erase(i); }

  system::iterator       system::begin()       { return m_bodies.begin(); }
  system::const_iterator system::begin() const { return m_bodies.begin(); }
  system::iterator       system::end()         { return m_bodies.end(); }
  system::const_iterator system::end()   const { return m_bodies.end(); }

  std::size_t system::size() const { return m_bodies.size(); }

  vector_momentum system::momentum() const {
    vector_momentum p = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      p += i->momentum();
    }
    return p;
  }

  scalar_energy system::kinetic_energy() const {
    scalar_energy E_k = 0;
    for (system::const_iterator i = begin(); i != end(); ++i) {
      for (body::const_iterator j = i->begin(); j != i->end(); ++j) {
        E_k += j->m()*norm(j->v())*norm(j->v())/2;
      }
    }
    return E_k;
  }

  void system::collision() {
    for (iterator i = begin(); i != end(); ++i) {
      for (iterator j = boost::next(i); j != end(); ++j) {
        carom::collision(*i, *j);
      }
    }
  }
}
