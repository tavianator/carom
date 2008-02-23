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
#include <list>
#include <vector>
#include <utility> // For pair

namespace carom
{
  vector_displacement mesh::center() const {
    // Find the geometric center of the mesh. This algorithm is O(n).
    vector_displacement r = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      r += i->s();
    }
    return r / size();
  }

  mesh::iterator mesh::inside(const vector_displacement& l0,
                              const vector_displacement& l1) {
    // Returns the first triangle in the mesh which l1 is inside of, if l1 is
    // inside the mesh. If l1 is inside any triangle in the mesh, and outside no
    // triangles in the mesh, then l1 is inside the mesh.

    iterator r = end();

    for (iterator i = begin(); i != end(); ++i) {
      intersection_info info(l0, l1, *i);

      if (info.inside() && r != end()) {
        r = i;
      } else if (info.outside()) {
        return end();
      }
    }

    return r;
  }

  mesh::iterator mesh::outside(const vector_displacement& l0,
                               const vector_displacement& l1) {
    // Returns the first triangle in the mesh which l1 is outside of.

    for (iterator i = begin(); i != end(); ++i) {
      if (intersection_info(l0, l1, *i).outside()) {
        return i;
      }
    }

    return end();
  }

  intersection_info::intersection_info(const vector_displacement& l0,
                                       const vector_displacement& l1,
                                       const triangle& p) {
    vector_displacement p0 = p.a()->s();
    vector_displacement p1 = p.b()->s();
    vector_displacement p2 = p.c()->s();

    m_t = dot(l0 - p0, cross(p1 - p0, p2 - p0))/
          dot(l0 - l1, cross(p1 - p0, p2 - p0));

    m_u = dot(l0 - l1, cross(l0 - p0, p2 - p0))/
          dot(l0 - l1, cross(p1 - p0, p2 - p0));

    m_v = dot(l0 - l1, cross(p1 - p0, l0 - p0))/
          dot(l0 - l1, cross(p1 - p0, p2 - p0));
  }

  bool intersection_info::inside() const {
    return (m_u >= 0 && m_v >= 0 && m_u + m_v <= 1 && m_t >= 1);
  }

  bool intersection_info::outside() const {
    return (m_u >= 0 && m_v >= 0 && m_u + m_v <= 1 && m_t >= 0 && m_t < 1);
  }

  namespace
  {
    body::iterator furthest_point(std::list<body::iterator>& remain,
                                  const vector_displacement& o) {
      std::list<body::iterator>::iterator i = remain.begin();
      scalar_distance r = norm((*i)->s() - o);
      for (std::list<body::iterator>::iterator j = boost::next(i);
           j != remain.end();
           ++j) {
        if (norm((*j)->s() - o) > r) {
          r = norm((*j)->s() - o);
          i = j;
        }
      }
      body::iterator ret = *i;
      remain.erase(i);
      return ret;
    }

    triangle clockwise_triangle(body::iterator a, body::iterator b,
                                body::iterator c,
                                const vector_displacement& o) {
      if (dot(a->s() - o, cross(b->s() - o, c->s() - o)) > 0) {
        return triangle(a, b, c);
      } else {
        return triangle(a, c, b);
      }
    }

    bool clockwise(const triangle& t, const vector_displacement& o) {
      return dot(t.a()->s() - o, cross(t.b()->s() - o, t.c()->s() - o)) > 0;
    }
  }

  mesh convex_hull(body& b) {
    mesh hull;

    if (b.size() >= 4) {
      std::list<body::iterator> remain;
      for (body::iterator i = b.begin(); i != b.end(); ++i) {
        remain.push_back(i);
      }

      vector_displacement o = center_of_mass(b);

      body::iterator p = furthest_point(remain, o);
      body::iterator q = furthest_point(remain, o);
      body::iterator r = furthest_point(remain, o);
      body::iterator s = furthest_point(remain, o);
      o = (p->s() + q->s() + r->s() + s->s())/4;
      hull.insert(clockwise_triangle(p, q, r, o));
      hull.insert(clockwise_triangle(p, q, s, o));
      hull.insert(clockwise_triangle(p, r, s, o));
      hull.insert(clockwise_triangle(q, r, s, o));

      while (!remain.empty()) {
        body::iterator i = furthest_point(remain, o);
        if (hull.outside(o, i->s()) != hull.end()) {
          std::list<mesh::iterator> visible;
          for (mesh::iterator j = hull.begin(); j != hull.end(); ++j) {
            if (!clockwise(*j, i->s())) {
              visible.push_back(j);
            }
          }
          
        }
      }
    }

    return hull;
  }
}
