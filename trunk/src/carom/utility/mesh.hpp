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

  class intersection_info
  {
  public:
    intersection_info(const vector_displacement& l0,
                      const vector_displacement& l1,
                      const triangle& p);

    bool inside() const;
    bool outside() const;

    scalar t() const { return m_t; }
    scalar u() const { return m_u; }
    scalar v() const { return m_v; }

  private:
    scalar m_t;
    scalar m_u;
    scalar m_v;
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

    std::size_t size() const { return m_triangles.size(); }

    vector_displacement center() const;

    // If l1 is inside the mesh, return the triangle it is inside of. Otherwise,
    // return end().
    iterator inside(const vector_displacement& l0,
                    const vector_displacement& l1);

    // If l1 is outside the mesh, return the triangle it is outside of.
    // Otherwise, return end().
    iterator outside(const vector_displacement& l0,
                     const vector_displacement& l1);

  private:
    std::list<triangle> m_triangles;

    mesh(const mesh&);
    mesh& operator=(const mesh&);
  };
}

#endif // CAROM_UTILITY_MESH_HPP
