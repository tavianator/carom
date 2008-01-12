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
#include <list>

namespace carom
{
  simple_f_base::~simple_f_base() {
  }

  k_base* simple_f_base::multiply(const scalar_time& t) const {
    simple_k_base* r = new simple_k_base;

    r->t = t;
    r->forces = forces;

    return r;
  }

  simple_k_base::~simple_k_base() {
  }

  k_base* simple_k_base::add(const k_base& k) const {
    simple_k_base* r = new simple_k_base;
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);

    r->t = t + rhs.t;
    r->forces = forces;

    return r;
  }

  k_base* simple_k_base::subtract(const k_base& k) const {
    simple_k_base* r = new simple_k_base;
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);

    r->t = t - rhs.t;
    r->forces = forces;

    return r;
  }

  k_base* simple_k_base::multiply(const scalar& n) const {
    simple_k_base* r = new simple_k_base;

    r->t = n*t;
    r->forces = forces;

    return r;
  }

  k_base* simple_k_base::divide(const scalar& n) const {
    simple_k_base* r = new simple_k_base;

    r->t = t/n;
    r->forces = forces;

    return r;
  }

  simple_y_base::~simple_y_base() {
  }

  y_base* simple_y_base::add(const k_base& k) const {
    simple_y_base* r = new simple_y_base;
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);

    body::const_iterator i;
    std::list<vector_force>::const_iterator j;
    for (i = b.begin(), j = rhs.forces.begin();
         i != b.end() && j != rhs.forces.end();
         ++i, ++j) {
      body::iterator k = r->b.insert(new particle()); 
      k->m(i->m());
      k->s(i->s() + rhs.t*i->v());
      k->v(i->v() + rhs.t*(*j)/i->m());
    }

    return r;
  }

  scalar simple_y_base::subtract(const y_base& y) const {
    scalar err = 0;
    const simple_y_base& rhs = dynamic_cast<const simple_y_base&>(y);

    for (body::const_iterator i = b.begin(), j = rhs.b.begin();
         i != b.end() && j != rhs.b.end();
         ++i, ++j) {
      err = std::max(err, std::max(convert<scalar>(norm(i->s() - j->s())),
                                   convert<scalar>(norm(i->v() - j->v()))));
    }

    return err;
  }

  f_value simple_body::f() {
    simple_f_base* r = new simple_f_base;

    apply_forces(*this);
    for (iterator i = begin(); i != end(); ++i) {
      r->forces.push_back(i->F());
    }

    return f_value(r);
  }

  y_value simple_body::y() {
    simple_y_base* r = new simple_y_base;

    for (iterator i = begin(); i != end(); ++i) {
      iterator j = r->b.insert(new particle());
      j->m(i->m());
      j->s(i->s());
      j->v(i->v());
    }

    return y_value(r);
  }

  body& simple_body::operator=(const y_value& y) {
    const simple_y_base& rhs = dynamic_cast<const simple_y_base&>(*y.base());

    iterator i;
    const_iterator j;
    for (i = begin(), j = rhs.b.begin(); i != end(); ++i, ++j) {
      i->s(j->s());
      i->v(j->v());
    }

    return *this;
  }
}
