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
  particle::~particle() { }

  scalar_mass         particle::m() const { return m_mass; }
  vector_displacement particle::s() const { return m_position; }
  vector_velocity     particle::v() const { return m_momentum / m_mass; }
  vector_momentum     particle::p() const { return m_momentum; }
  vector_acceleration particle::a() const { return m_force / m_mass; }
  vector_force        particle::F() const { return m_force; }

  void particle::m(const scalar_mass& m)         { m_mass = m; }
  void particle::s(const vector_displacement& s) { m_position = s; }
  void particle::v(const vector_velocity& v)     { m_momentum = m_mass*v; }
  void particle::p(const vector_momentum& p)     { m_momentum = p; }
  void particle::a(const vector_acceleration& a) { m_force = m_mass*a; }
  void particle::F(const vector_force& F)        { m_force = F; }

  particle::iterator particle::apply_force(applied_force* force) {
    return m_forces.insert(m_forces.end(), force);
  }

  void particle::remove_force(iterator i) { m_forces.erase(i); }

  particle::iterator       particle::begin()       { return m_forces.begin(); }
  particle::const_iterator particle::begin() const { return m_forces.begin(); }
  particle::iterator       particle::end()         { return m_forces.end(); }
  particle::const_iterator particle::end()   const { return m_forces.end(); }

  std::size_t particle::size() const { return m_forces.size(); }

  void particle::apply_forces() {
    m_force = 0;

    for (iterator i = m_forces.begin(); i != m_forces.end(); ++i) {
      m_force += i->force(*this);
    }
  }
}
