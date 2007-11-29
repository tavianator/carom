#include <carom.hpp>

namespace carom
{
  intersection_info::intersection_info(const vector_displacement& l0,
                                       const vector_displacement& l1,
                                       const triangle& p) {
    vector_displacement p0 = p.a->s();
    vector_displacement p1 = p.b->s();
    vector_displacement p2 = p.c->s();

    t = dot(l0 - p0, cross(p1 - p0, p2 - p0))/
        dot(l0 - l1, cross(p1 - p0, p2 - p0));

    u = dot(l0 - l1, cross(l0 - p0, p2 - p0))/
        dot(l0 - l1, cross(p1 - p0, p2 - p0));

    v = dot(l0 - l1, cross(p1 - p0, l0 - p0))/
        dot(l0 - l1, cross(p1 - p0, p2 - p0));
  }

  bool intersection_info::inside() const {
    return (u >= 0 && v >= 0 && u + v <= 0 && t >= 1);
  }

  bool intersection_info::outside() const {
    return (u >= 0 && v >= 0 && u + v <= 0 && t >= 0 && t < 1);
  }

  vector_displacement mesh::center() const {
    // Find the geometric center of the mesh. This algorithm is O(n).

    vector_displacement r = 0;

    for (const_iterator i = begin(); i != end(); ++i) {
      r += (i->a->s() + i->b->s() + i->c->s())/3;
    }

    return r / size();
  }

  bool mesh::inside(const vector_displacement& l0,
                    const vector_displacement& l1) const {
    return !outside(l0, l1);
  }

  bool mesh::outside(const vector_displacement& l0,
                     const vector_displacement& l1) const {
    // A line through l0 and l1 intersects a triangle iff u >= 0, v >= 0,
    // and (u + v) <= 1. l1 is outside the triangle iff t >= 0 and t < 1. If,
    // for any triangle in the mesh, these conditions are met, then l1 is
    // outside the mesh. This algorithm is O(n).

    intersection_info info;

    for (const_iterator i = begin(); i != end(); ++i) {
      if (intersection_info(l0, l1, *i).outside()) {
        return true;
      }
    }

    return false;
  }
}
