#include <carom.hpp>

namespace carom
{
  intersection_info::intersection_info(const vector_displacement& l0,
                                       const vector_displacement& l1,
                                       const triangle& p) {
    vector_displacement p0 = p.a->s();
    vector_displacement p1 = p.b->s();
    vector_displacement p2 = p.c->s();

    m_t = dot(l0 - p0, cross(p1 - p0, p2 - p0))/
          dot(l0 - l1, cross(p1 - p0, p2 - p0));

    m_u = dot(l0 - l1, cross(l0 - p0, p2 - p0))/
          dot(l0 - l1, cross(p1 - p0, p2 - p0));

    m_v = dot(l0 - l1, cross(p1 - p0, l0 - p0))/
          dot(l0 - l1, cross(p1 - p0, p2 - p0));
  }

  bool intersection_info::inside() const {
    return (m_u >= 0 && m_v >= 0 && m_u + m_v <= 0 && m_t >= 1);
  }

  bool intersection_info::outside() const {
    return (m_u >= 0 && m_v >= 0 && m_u + m_v <= 0 && m_t >= 0 && m_t < 1);
  }

  vector_displacement mesh::center() const {
    // Find the geometric center of the mesh. This algorithm is O(n).

    vector_displacement r = 0;

    for (const_iterator i = begin(); i != end(); ++i) {
      r += (i->a->s() + i->b->s() + i->c->s())/3;
    }

    return r / size();
  }

  mesh::iterator mesh::inside(const vector_displacement& l0,
                              const vector_displacement& l1) {
    // If l1 is inside any triangle in the mesh, and outside no triangles in
    // the mesh, then l1 is inside the mesh.

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
    // If l1 is outside any triangle in the mesh, then it is outside the mesh.

    for (iterator i = begin(); i != end(); ++i) {
      if (intersection_info(l0, l1, *i).outside()) {
        return i;
      }
    }

    return end();
  }
}
