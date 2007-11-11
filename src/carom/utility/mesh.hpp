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

#ifndef CAROM_UTILITY_MESH_HPP
#define CAROM_UTILITY_MESH_HPP

#include <list>

namespace carom
{
  struct triangle
  {
  public:
    body::iterator a;
    body::iterator b;
    body::iterator c;
  };

  class mesh
  {
  public:
    typedef std::list<triangle>::iterator iterator;
    typedef std::list<triangle>::const_iterator const_iterator;

    mesh() { }
    // ~mesh();

    iterator       begin()       { return m_triangles.begin(); }
    const_iterator begin() const { return m_triangles.begin(); }
    iterator       end()         { return m_triangles.end(); }
    const_iterator end() const   { return m_triangles.end(); }

  private:
    std::list<triangle> m_triangles;

    mesh(const mesh&);
    mesh& operator=(const mesh&);
  };
}

#endif // CAROM_UTILITY_MESH_HPP
