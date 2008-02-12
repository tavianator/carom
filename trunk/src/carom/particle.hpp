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

#include <boost/utility.hpp> // For noncopyable

namespace carom
{
  class particle;

  class applied_force : private boost::noncopyable
  {
  public:
    // applied_force();
    virtual ~applied_force() { }

    virtual vector_force force(const particle& x) const = 0;
  };

  class particle : private boost::noncopyable
  {
  public:
    typedef polymorphic_list<applied_force>::iterator       iterator;
    typedef polymorphic_list<applied_force>::const_iterator const_iterator;

    // particle();
    virtual ~particle();

    scalar_mass         m() const;
    vector_displacement s() const;
    vector_velocity     v() const;
    vector_momentum     p() const;
    vector_acceleration a() const;
    vector_force        F() const;

    void m(const scalar_mass& m);
    void s(const vector_displacement& s);
    void v(const vector_velocity& v);
    void p(const vector_momentum& p);
    void a(const vector_acceleration& a);
    void F(const vector_force& F);

    iterator apply_force(applied_force* force);
    void remove_force(iterator i);

    iterator       begin();
    const_iterator begin() const;
    iterator       end();
    const_iterator end()   const;

    std::size_t size() const;

    void apply_forces();

  private:
    scalar_mass         m_mass;
    vector_displacement m_position;
    vector_momentum     m_momentum;
    vector_force        m_force;

    polymorphic_list<applied_force> m_forces;
  };
}

#endif // CAROM_PARTICLE_HPP
