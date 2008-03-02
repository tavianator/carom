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
  bool triangle::clockwise(const vector_displacement& r) const {
    return dot(a()->s() - r, cross(b()->s() - r, c()->s() - r)) > 0;
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

  vector_displacement mesh::center() const {
    // Find the geometric center of the mesh. This algorithm is O(n).
    vector_displacement r = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      r += i->s();
    }
    return r / size();
  }

  std::list<mesh::iterator>
  mesh::visible(const vector_displacement& o, const vector_displacement& r) {
    std::list<mesh::iterator> v;
    for (iterator i = begin(); i != end(); ++i) {
      if (i->clockwise(o) != i->clockwise(r)) {
        v.push_back(i);
      }
    }
    return v;
  }

  mesh::iterator mesh::inside(const vector_displacement& l0,
                              const vector_displacement& l1) {
    // Returns the first triangle in the mesh which l1 is inside of, if l1 is
    // inside the mesh. If l1 is inside any triangle in the mesh, and outside no
    // triangles in the mesh, then l1 is inside the mesh.

    iterator r = end();

    for (iterator i = begin(); i != end(); ++i) {
      intersection_info info(l0, l1, *i);

      if (info.inside()) {
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

  namespace
  {
    // Strict weak ordering on clockwiseness.
    class clockwise_pred
    {
    public:
      clockwise_pred(const vector_displacement& o,
                     body::iterator i, body::iterator ref)
        : m_o(o), m_i(i), m_ref(ref) { }

      // clockwise_pred(const clockwise_pred& pred);
      // ~clockwise_pred();

      // clockwise_pred& operator=(const clockwise_pred& pred);

      bool operator()(body::iterator a, body::iterator b) const {
        //    ref
        //  O     I
        // N       J
        //  M     K
        //     L
        //
        // ref < I < J < K < L < M < M < O

        if (a == b) {
          return false;
        } else if (a == m_ref) {
          return true;
        } else if (b == m_ref) {
          return false;
        } else if (triangle(m_ref, a, m_i).clockwise(m_o)) {
          return triangle(a, b, m_i).clockwise(m_o) ||
            (!triangle(m_ref, b, m_i).clockwise(m_o));
        } else {
          return triangle(a, b, m_i).clockwise(m_o) &&
            triangle(b, m_ref, m_i).clockwise(m_o);
        }
      }

    private:
      vector_displacement m_o;
      body::iterator m_i, m_ref;
    };
  }

  mesh convex_hull(body& b) {
    mesh hull;

    if (b.size() >= 4) {
      std::list<body::iterator> remain;
      for (body::iterator i = b.begin(); i != b.end(); ++i) {
        remain.push_back(i);
      }

      body::iterator p = remain.front(); remain.pop_front();
      body::iterator q = remain.front(); remain.pop_front();
      body::iterator r = remain.front(); remain.pop_front();
      body::iterator s = remain.front(); remain.pop_front();
      hull.insert(triangle(p, q, r));
      hull.insert(triangle(p, q, s));
      hull.insert(triangle(p, r, s));
      hull.insert(triangle(q, r, s));

      vector_displacement o = hull.center();

      while (!remain.empty()) {
        body::iterator i = remain.front();

        std::list<body::iterator> base, removed;
        std::list<mesh::iterator> visible = hull.visible(o, i->s());
        for (std::list<mesh::iterator>::iterator j = visible.begin();
             j != visible.end();
             ++j) {
          removed.push_back((*j)->a());
          removed.push_back((*j)->b());
          removed.push_back((*j)->c());
          hull.erase(*j);
        }
        for (std::list<body::iterator>::iterator j = removed.begin();
             j != removed.end();
             ++j) {
          for (mesh::iterator k = hull.begin(); k != hull.end(); ++k) {
            if (*j == k->a() || *j == k->b() || *j == k->c()) {
              base.push_back(*j);
              break;
            }
          }
        }
        base.sort(clockwise_pred(o, i, base.front()));
        base.unique();
        if (base.size() > 1) {
          for (std::list<body::iterator>::iterator j = base.begin();
               j != boost::prior(base.end());
               ++j) {
            hull.insert(triangle(*j, *boost::next(j), i));
          }
          hull.insert(triangle(base.back(), base.front(), i));
        }

        remain.pop_front();
      }
    }

    return hull;
  }
}
