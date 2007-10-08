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

  struct particle
  {
  public:
    particle() { }
    // ~particle();

    scalar_mass         m;
    vector_displacement s;
    vector_momentum     p;
    vector_force        F;

    vector_velocity     v() { return p / m; }
    vector_acceleration a() { return F / m; }

    void apply_force(applied_force* force) { m_forces.push_back(force); }
    void apply_forces();
    void clear_forces() { m_forces.clear(); }

  private:
    noncopyable_list<applied_force> m_forces;

    particle(const particle&);
    particle& operator=(const particle&);
  };
}

#endif // CAROM_PARTICLE_HPP
