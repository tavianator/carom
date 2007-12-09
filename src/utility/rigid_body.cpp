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

  void rigid_body::calculate_k1() {
    apply_forces(*this);
    m_m = mass(*this);
    m_o = center_of_mass(*this);
    m_p = momentum(*this);
    m_L = angular_momentum(*this, m_o);
    m_F1 = force(*this);
    m_T1 = torque(*this, m_o);
  }

  void rigid_body::calculate_k2() {
    apply_forces(*this);
    m_F2 = force(*this);
    m_T2 = torque(*this, m_o);
  }

  void rigid_body::calculate_k3() {
    apply_forces(*this);
    m_F3 = force(*this);
    m_T3 = torque(*this, m_o);
  }

  void rigid_body::calculate_k4() {
    apply_forces(*this);
    m_F4 = force(*this);
    m_T4 = torque(*this, m_o);
  }

  void rigid_body::apply_k1(const scalar_time& t) {
    advance(t, m_F1, m_T1);
  }

  void rigid_body::apply_k2(const scalar_time& t) {
    retreat();
    advance(t, m_F2, m_T2);
  }

  void rigid_body::apply_k3(const scalar_time& t) {
    retreat();
    advance(t, m_F3, m_T3);
  }

  void rigid_body::apply(const scalar_time& t) {
    retreat();
    advance(t, (m_F1 + 2*m_F2 + 2*m_F3 + m_F4)/6,
            (m_T1 + 2*m_T2 + 2*m_T3 + m_T4)/6);
    clear_forces(*this);
  }

  void rigid_body::advance(const scalar_time& t, const vector_force& F,
                           const vector_torque& T) {
    m_s = (m_p*t + F*t*t/2)/m_m;

    if (m_L*t + T*t*t/2 == 0) {
      m_theta = 0;

      for (iterator i = begin(); i != end(); ++i) {
        i->s(i->s() + m_s);
        i->p(m_p + F*t);
      }
    } else {
      m_I = moment_of_inertia(*this, m_o, normalized(m_L*t + T*t*t/2));
      m_theta = (m_L*t + T*t*t/2)/m_I;

      for (iterator i = begin(); i != end(); ++i) {
        i->s(rotate(i->s(), m_o, m_theta) + m_s);
        i->v((m_p + F*t)/m_m + cross((m_L + T*t)/m_I, i->s() - m_o));
      }
    }
  }

  void rigid_body::retreat() {
    if (m_theta == 0) {
      for (iterator i = begin(); i != end(); ++i) {
        i->s(i->s() - m_s);
        i->p(m_p);
      }
    } else {
      for (iterator i = begin(); i != end(); ++i) {
        i->s(rotate(i->s() - m_s, m_o, -m_theta));
        i->v(m_p/m_m + cross(m_L/m_I, i->s() - m_o));
      }
    }
  }
}
