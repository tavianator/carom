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
  rigid_f_base::rigid_f_base(const vector_force& F, const vector_torque& T)
    : m_F(F), m_T(T) { }

  rigid_f_base::~rigid_f_base() { }

  vector_force  rigid_f_base::F() const { return m_F; }
  vector_torque rigid_f_base::T() const { return m_T; }
  void rigid_f_base::F(const vector_force& F)  { m_F = F; }
  void rigid_f_base::T(const vector_torque& T) { m_T = T; }

  k_base* rigid_f_base::multiply(const scalar_time& dt) const {
    return new rigid_k_base(dt, dt*F(), dt*T());
  }

  rigid_k_base::rigid_k_base(const scalar_time& dt,
                             const vector_momentum& dp,
                             const vector_angular_momentum& dL)
    : m_dt(dt), m_dp(dp), m_dL(dL) { }
  rigid_k_base::~rigid_k_base() {
  }

  scalar_time             rigid_k_base::dt() const { return m_dt; }
  vector_momentum         rigid_k_base::dp() const { return m_dp; }
  vector_angular_momentum rigid_k_base::dL() const { return m_dL; }
  void rigid_k_base::dt(const scalar_time& dt)             { m_dt = dt; }
  void rigid_k_base::dp(const vector_momentum& dp)         { m_dp = dp; }
  void rigid_k_base::dL(const vector_angular_momentum& dL) { m_dL = dL; }

  k_base* rigid_k_base::add(const k_base& k) const {
    const rigid_k_base& rhs = dynamic_cast<const rigid_k_base&>(k);
    return new rigid_k_base(dt() + rhs.dt(), dp() + rhs.dp(), dL() + rhs.dL());
  }

  k_base* rigid_k_base::subtract(const k_base& k) const {
    const rigid_k_base& rhs = dynamic_cast<const rigid_k_base&>(k);
    return new rigid_k_base(dt() - rhs.dt(), dp() - rhs.dp(), dL() - rhs.dL());
  }

  k_base* rigid_k_base::multiply(const scalar& n) const {
    return new rigid_k_base(n*dt(), n*dp(), n*dL());
  }

  k_base* rigid_k_base::divide(const scalar& n) const {
    return new rigid_k_base(dt()/n, dp()/n, dL()/n);
  }

  scalar_moment_of_inertia
  rigid_body::moment_of_inertia(const vector_displacement& o,
                                const vector& axis) const {
    scalar_moment_of_inertia I = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      scalar_distance r = norm(i->s() - o - proj(axis, i->s() - o));
      I += i->m()*r*r;
    }
    return I;
  }

  vector_angular_velocity
  rigid_body::angular_velocity(const vector_displacement& o,
                               const vector& axis) const {
    return angular_momentum(o)/moment_of_inertia(o, axis);
  }

  vector_angular_momentum
  rigid_body::angular_momentum(const vector_displacement& o) const {
    vector_angular_momentum L = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      L += cross(i->s() - o, i->p());
    }
    return L;
  }

  vector_angular_acceleration
  rigid_body::angular_acceleration(const vector_displacement& o,
                                   const vector& axis) const {
    return torque(o)/moment_of_inertia(o, axis);
  }

  vector_torque rigid_body::torque(const vector_displacement& o) const {
    vector_torque T = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      T += cross(i->s() - o, i->F());
    }
    return T;
  }

  scalar_mass rigid_body::mass(const particle& x) const {
    return mass();
  }

  void rigid_body::collision(particle& x, const vector_momentum& dp) {
    scalar_mass m = mass();
    vector_displacement o = center_of_mass();
    vector_momentum p = momentum();
    vector_angular_momentum L = angular_momentum(o);
    vector_angular_momentum dL = cross(x.s() - o, dp);

    if (L + dL == 0) {
      for (iterator i = begin(); i != end(); ++i) {
        i->v((p + dp)/m);
      }
    } else {
      scalar_moment_of_inertia I =
        moment_of_inertia(o, normalized(L + dL));

      for (iterator i = begin(); i != end(); ++i) {
        i->v((p + dp)/m + cross((L + dL)/I, i->s() - o));
      }
    }
  }

  f_value rigid_body::f() {
    apply_forces();
    rigid_f_base* r = new rigid_f_base(force(), torque(center_of_mass()));
    return f_value(r);
  }

  y_value rigid_body::y() {
    y_base* r = new y_base();

    r->backup(new rigid_body());
    for (iterator i = begin(); i != end(); ++i) {
      iterator j = r->backup()->insert(new particle());
      j->m(i->m());
      j->s(i->s());
      j->p(i->p());
    }

    return y_value(r);
  }

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

  void rigid_body::step(const y_value& y0, const k_value& kv) {
    const rigid_k_base& kval = dynamic_cast<const rigid_k_base&>(*kv.base());
    const rigid_body& backup =
      dynamic_cast<const rigid_body&>(*y0.base()->backup());

    scalar_mass m = backup.mass();
    vector_displacement o = backup.center_of_mass();
    vector_momentum p = backup.momentum();
    vector_angular_momentum L = backup.angular_momentum(o);
    vector_displacement ds = kval.dt()*(p + kval.dp()/2)/m;

    if (kval.dt()*(L + kval.dL()/2) == 0) {
      iterator i = begin();
      const_iterator j = backup.begin();
      for (; i != end(); ++i, ++j) {
        i->s(j->s() + ds);
        i->v((p + kval.dp())/m);
      }
    } else {
      scalar_moment_of_inertia I =
        backup.moment_of_inertia(o, normalized(kval.dt()*(L + kval.dL()/2)));
      vector_angle theta = kval.dt()*(L + kval.dL()/2)/I;

      iterator i = begin();
      const_iterator j = backup.begin();
      for (; i != end(); ++i, ++j) {
        i->s(rotate(j->s(), o, theta) + ds);
        i->v((p + kval.dp())/m + cross((L + kval.dL())/I, i->s() - o - ds));
      }
    }
  }
 
  template <>
  scalar_mass impenetrable_body<rigid_body>::mass(const triangle& t) {
    return mass();
  }
 
  template <>
  void impenetrable_body<rigid_body>::collision(const triangle& t,
                                                const vector_momentum& dp) {
    scalar_mass m = mass();
    vector_displacement o = center_of_mass();
    vector_displacement x = (t.a()->s() + t.b()->s() + t.c()->s())/3;
    vector_momentum p = momentum();
    vector_angular_momentum L = angular_momentum(o);
    vector_angular_momentum dL = cross(x - o, dp);

    if (L + dL == 0) {
      for (iterator i = begin(); i != end(); ++i) {
        i->v((p + dp)/m);
      }
    } else {
      scalar_moment_of_inertia I =
        moment_of_inertia(o, normalized(L + dL));

      for (iterator i = begin(); i != end(); ++i) {
        i->v((p + dp)/m + cross((L + dL)/I, i->s() - o));
      }
    }
  }
}
