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
#include <algorithm> // For max()
#include <vector>

namespace carom
{
  namespace
  {
    vector_displacement rotate(const vector_displacement& v,
                               const vector_displacement& o,
                               const vector_angle& theta) {
      // Rotating a vector v around a normalized axis u by an angle theta gives
      // v*cos(theta) + cross(u, v)*sin(theta) + dot(u, v)*u*(1 - cos(theta))
      if (norm(theta) != 0) {
        scalar_angle angle = norm(theta);
        vector axis = normalized(theta);
        vector_displacement r = v - o;

        return r*cos(angle) + cross(axis, r)*sin(angle) +
          dot(axis, r)*axis*(1 - cos(angle)) + o;
      } else {
        return v;
      }
    }
  }

  rigid_f_base::~rigid_f_base() {
  }

  k_base* rigid_f_base::multiply(const scalar_time& dt) const {
    rigid_k_base* r = new rigid_k_base;

    r->dt = dt;
    r->dp = dt*F;
    r->dL = dt*T;

    return r;
  }

  rigid_k_base::~rigid_k_base() {
  }

  k_base* rigid_k_base::add(const k_base& k) const {
    rigid_k_base* r = new rigid_k_base;
    const rigid_k_base& rhs = dynamic_cast<const rigid_k_base&>(k);

    r->dt = dt + rhs.dt;
    r->dp = dp + rhs.dp;
    r->dL = dL + rhs.dL;

    return r;
  }

  k_base* rigid_k_base::subtract(const k_base& k) const {
    rigid_k_base* r = new rigid_k_base;
    const rigid_k_base& rhs = dynamic_cast<const rigid_k_base&>(k);

    r->dt = dt - rhs.dt;
    r->dp = dp - rhs.dp;
    r->dL = dL - rhs.dL;

    return r;
  }

  k_base* rigid_k_base::multiply(const scalar& n) const {
    rigid_k_base* r = new rigid_k_base;

    r->dt = n*dt;
    r->dp = n*dp;
    r->dL = n*dL;

    return r;
  }

  k_base* rigid_k_base::divide(const scalar& n) const {
    rigid_k_base* r = new rigid_k_base;

    r->dt = dt/n;
    r->dp = dp/n;
    r->dL = dL/n;

    return r;
  }

  rigid_y_base::~rigid_y_base() {
  }

  y_base* rigid_y_base::add(const k_base& k) const {
    rigid_y_base* r = new rigid_y_base;
    const rigid_k_base& rhs = dynamic_cast<const rigid_k_base&>(k);

    r->dt = dt + rhs.dt;
    r->dp = dp + rhs.dp;
    r->dL = dL + rhs.dL;
    r->backup = backup;

    return r;
  }

  scalar rigid_y_base::subtract(const y_base& y) const {
    const rigid_y_base& rhs = dynamic_cast<const rigid_y_base&>(y);
    return std::max(convert<scalar>(norm(dp - rhs.dp)),
                    convert<scalar>(norm(dL - rhs.dL)));
  }

  f_value rigid_body::f() {
    rigid_f_base* r = new rigid_f_base;

    apply_forces(*this);
    r->F = force(*this);
    r->T = torque(*this, center_of_mass(*this));

    return f_value(r);
  }

  y_value rigid_body::y() {
    rigid_y_base* r = new rigid_y_base;

    r->dt = 0;
    r->dp = 0;
    r->dL = 0;
    r->backup.reset(new body());
    for (iterator i = begin(); i != end(); ++i) {
      iterator j = r->backup->insert(new particle());
      j->m(i->m());
      j->s(i->s());
      j->v(i->v());
    }

    return y_value(r);
  }

  body& rigid_body::operator=(const y_value& y) {
    const rigid_y_base& rhs = dynamic_cast<const rigid_y_base&>(*y.base());

    scalar_mass m = mass(*rhs.backup);
    vector_displacement o = center_of_mass(*rhs.backup);
    vector_momentum p = momentum(*rhs.backup);
    vector_angular_momentum L = angular_momentum(*rhs.backup, o);
    vector_displacement ds = rhs.dt*(p + rhs.dp/2)/m;

    if (rhs.dt*(L + rhs.dL/2) == 0) {
      iterator i = begin();
      const_iterator j = rhs.backup->begin();
      for (; i != end(); ++i, ++j) {
        i->s(j->s() + ds);
        i->v((p + rhs.dp)/m);
      }
    } else {
      scalar_moment_of_inertia I =
        moment_of_inertia(*this, o, normalized(rhs.dt*(L + rhs.dL/2)));
      vector_angle theta = rhs.dt*(L + rhs.dL/2)/I;

      iterator i = begin();
      const_iterator j = rhs.backup->begin();
      for (; i != end(); ++i, ++j) {
        i->s(rotate(j->s(), o, theta) + ds);
        i->v((p + rhs.dp)/m + cross((L + rhs.dL)/I, i->s() - o));
      }
    }

    return *this;
  }

  template <>
  void impenetrable_body<rigid_body>::collision(mesh::iterator i,
                                                const vector_momentum& dp) {
    scalar_mass m = mass(*this);
    vector_displacement o = center_of_mass(*this);
    vector_displacement x = (i->a->s() + i->b->s() + i->c->s())/3;
    vector_momentum p = momentum(*this);
    vector_angular_momentum L = angular_momentum(*this, o);
    vector_angular_momentum dL = cross(x - o, dp);

    if (L + dL == 0) {
      for (iterator i = begin(); i != end(); ++i) {
        i->v((p + dp)/m);
      }
    } else {
      scalar_moment_of_inertia I =
        moment_of_inertia(*this, o, normalized(L + dL));

      for (iterator i = begin(); i != end(); ++i) {
        i->v((p + dp)/m + cross((L + dL)/I, i->s() - o));
      }
    }
  }
}
