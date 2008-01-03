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

#ifndef CAROM_SYSTEM_HPP
#define CAROM_SYSTEM_HPP

#include <boost/utility.hpp> // For noncopyable

namespace carom
{
  class system : private boost::noncopyable
  {
  public:
    typedef polymorphic_list<body>::iterator       iterator;
    typedef polymorphic_list<body>::const_iterator const_iterator;

    // system();
    // ~system();

    iterator insert(body* b) { return m_bodies.insert(m_bodies.end(), b); }
    void erase(iterator i) { m_bodies.erase(i); }

    iterator       begin()       { return m_bodies.begin(); }
    const_iterator begin() const { return m_bodies.begin(); }
    iterator       end()         { return m_bodies.end(); }
    const_iterator end() const   { return m_bodies.end(); }

    std::size_t size() const { return m_bodies.size(); }

    void        integrate_Euler   (const scalar_time& t);
    void        integrate_midpoint(const scalar_time& t);
    void        integrate_RK4     (const scalar_time& t);
    scalar_time integrate_DP      (const scalar_time& t, scalar_time& elapsed);

    void collision();

  private:
    polymorphic_list<body> m_bodies;
  };
}

#endif // CAROM_SYSTEM_HPP
