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

#ifndef CAROM_PARTICLE_HPP
#define CAROM_PARTICLE_HPP

namespace carom
{
  class applied_force
  {
  public:
    applied_force() { }
    virtual ~applied_force() { }

    virtual vector_force force() = 0;

  private:
    applied_force(const applied_force&);
    applied_force& operator=(const applied_force&);
  };

  class particle
  {
  public:
    particle() { }
    virtual ~particle() { }

    scalar_mass         m() const { return m_mass; }
    vector_displacement s() const { return m_position; }
    vector_velocity     v() const { return m_momentum / m_mass; }
    vector_momentum     p() const { return m_momentum; }
    vector_acceleration a() const { return m_force / m_mass; }
    vector_force        F() const { return m_force; }

    void m(const scalar_mass& m)         { m_mass = m; }
    void s(const vector_displacement& s) { m_position = s; }
    void v(const vector_velocity& v)     { m_momentum = v * m_mass; }
    void p(const vector_momentum& p)     { m_momentum = p; }
    void a(const vector_acceleration& a) { m_force = a * m_mass; }
    void F(const vector_force& F)        { m_force = F; }

    void apply_force(applied_force* force) { m_forces.push_back(force); }
    void apply_forces();
    void clear_forces() { m_forces.clear(); }

  private:
    scalar_mass         m_mass;
    vector_displacement m_position;
    vector_momentum     m_momentum;
    vector_force        m_force;

    polymorphic_list<applied_force> m_forces;

    particle(const particle&);
    particle& operator=(const particle&);
  };
}

#endif // CAROM_PARTICLE_HPP
