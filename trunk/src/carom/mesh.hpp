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

#ifndef CAROM_MESH_HPP
#define CAROM_MESH_HPP

#include <boost/utility.hpp>
#include <list>

namespace carom
{
  class triangle
  {
  public:
    triangle(body::iterator a, body::iterator b, body::iterator c)
      : m_a(a), m_b(b), m_c(c) { }

    // triangle(const triangle& t);
    // ~triangle();

    // triangle& operator=(const triangle& t);

    body::iterator a() const { return m_a; }
    body::iterator b() const { return m_b; }
    body::iterator c() const { return m_c; }

    vector_displacement s() const { return (a()->s() + b()->s() + c()->s())/3; }
    vector_momentum     p() const { return a()->p() + b()->p() + c()->p(); }
    scalar_mass         m() const { return a()->m() + b()->m() + c()->m(); }
    vector_velocity     v() const { return p()/m(); }

    bool clockwise(const vector_displacement& r) const;

  private:
    body::iterator m_a, m_b, m_c;
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
    typedef std::list<triangle>::iterator       iterator;
    typedef std::list<triangle>::const_iterator const_iterator;

    // mesh();
    // mesh(const mesh& m);
    // ~mesh();

    // mesh& operator=(const mesh& m);

    iterator insert(const triangle& t)
    { return m_triangles.insert(m_triangles.end(), t); }
    void erase(iterator i) { m_triangles.erase(i); }

    iterator       begin()       { return m_triangles.begin(); }
    const_iterator begin() const { return m_triangles.begin(); }
    iterator       end()         { return m_triangles.end(); }
    const_iterator end()   const { return m_triangles.end(); }

    std::size_t size() const { return m_triangles.size(); }

    vector_displacement center() const;

    std::list<mesh::iterator>
    visible(const vector_displacement& o, const vector_displacement& r);

    iterator inside (const vector_displacement& l0,
                     const vector_displacement& l1);
    iterator outside(const vector_displacement& l0,
                     const vector_displacement& l1);

  private:
    std::list<triangle> m_triangles;
  };

  mesh convex_hull(body& b);
}

#endif // CAROM_MESH_HPP
