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
  rigid_f_base::~rigid_f_base() {
  }

  k_base* rigid_f_base::multiply(const scalar_time& t) const {
    rigid_k_base* r = new rigid_k_base;

    r->t = t;
    r->p = t*F;
    r->L = t*T;

    return r;
  }

  rigid_k_base::~rigid_k_base() {
  }

  k_base* rigid_k_base::add(const k_base& k) const {
    rigid_k_base* r = new rigid_k_base;
    const rigid_k_base& rhs = dynamic_cast<const rigid_k_base&>(k);

    r->t = t + rhs.t;
    r->p = p + rhs.p;
    r->L = L + rhs.L;

    return r;
  }

  k_base* rigid_k_base::subtract(const k_base& k) const {
    rigid_k_base* r = new rigid_k_base;
    const rigid_k_base& rhs = dynamic_cast<const rigid_k_base&>(k);

    r->t = t - rhs.t;
    r->p = p - rhs.p;
    r->L = L + rhs.L;

    return r;
  }

  k_base* rigid_k_base::multiply(const scalar& n) const {
    rigid_k_base* r = new rigid_k_base;

    r->t = n*t;
    r->p = n*p;
    r->L = n*L;

    return r;
  }

  k_base* rigid_k_base::divide(const scalar& n) const {
    rigid_k_base* r = new rigid_k_base;

    r->t = t/n;
    r->p = p/n;
    r->L = L/n;

    return r;
  }

  rigid_y_base::~rigid_y_base() {
  }

  y_base* rigid_y_base::add(const k_base& k) const {
    rigid_y_base* r = new rigid_y_base;
    const rigid_k_base& rhs = dynamic_cast<const rigid_k_base&>(k);

    r->t = t + rhs.t;
    r->p = p + rhs.p;
    r->L = L + rhs.L;
    r->backup = backup;

    return r;
  }

  scalar rigid_y_base::subtract(const y_base& y) const {
    const rigid_y_base& rhs = dynamic_cast<const rigid_y_base&>(y);
    return std::max(convert<scalar>(norm(p - rhs.p)),
                    convert<scalar>(norm(L - rhs.L)));
  }

  f_value rigid_body::f() {
    rigid_f_base* r = new rigid_f_base;

    apply_forces(*this);
    r->F = force(*this);
    r->T = torque(*this);

    return f_value(r);
  }

  y_value rigid_body::y() {
    rigid_y_base* r = new rigid_y_base;

    r->t = 0;
    r->p = 0;
    r->L = 0;
    r->backup = new body();
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

    iterator j = begin();
    const_iterator k = rhs.backup->begin();
    for (unsigned int i = 0; i < rhs.momenta.size(); ++i, ++j, ++k) {
      j->s(k->s() + rhs.t*(k->v() + rhs.momenta[i]/j->m()/2));
      j->v(k->v() + rhs.momenta[i]/j->m());
    }

    return *this;
  }
}
