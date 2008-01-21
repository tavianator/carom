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
  simple_f_base::~simple_f_base() {
  }

  k_base* simple_f_base::multiply(const scalar_time& t) const {
    simple_k_base* r = new simple_k_base;

    r->t = t;
    r->momenta.resize(forces.size());
    for (unsigned int i = 0; i < forces.size(); ++i) {
      r->momenta[i] = t*forces[i];
    }

    return r;
  }

  simple_k_base::~simple_k_base() {
  }

  k_base* simple_k_base::add(const k_base& k) const {
    simple_k_base* r = new simple_k_base;
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);

    r->t = t + rhs.t;
    r->momenta.resize(momenta.size());
    for (unsigned int i = 0; i < momenta.size(); ++i) {
      r->momenta[i] = momenta[i] + rhs.momenta[i];
    }

    return r;
  }

  k_base* simple_k_base::subtract(const k_base& k) const {
    simple_k_base* r = new simple_k_base;
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);

    r->t = t - rhs.t;
    r->momenta.resize(momenta.size());
    for (unsigned int i = 0; i < momenta.size(); ++i) {
      r->momenta[i] = momenta[i] - rhs.momenta[i];
    }

    return r;
  }

  k_base* simple_k_base::multiply(const scalar& n) const {
    simple_k_base* r = new simple_k_base;

    r->t = n*t;
    r->momenta.resize(momenta.size());
    for (unsigned int i = 0; i < momenta.size(); ++i) {
      r->momenta[i] = n*momenta[i];
    }

    return r;
  }

  k_base* simple_k_base::divide(const scalar& n) const {
    simple_k_base* r = new simple_k_base;

    r->t = t/n;
    r->momenta.resize(momenta.size());
    for (unsigned int i = 0; i < momenta.size(); ++i) {
      r->momenta[i] = momenta[i]/n;
    }

    return r;
  }

  simple_y_base::~simple_y_base() {
  }

  y_base* simple_y_base::add(const k_base& k) const {
    simple_y_base* r = new simple_y_base;
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);

    r->t = t + rhs.t;
    r->momenta.resize(momenta.size());
    body::const_iterator j = backup.begin();
    for (unsigned int i = 0; i < momenta.size(); ++i, ++j) {
      body::iterator k = r->backup.insert(new particle());
      k->m(j->m());
      k->s(j->s());
      k->v(j->v());
      r->momenta[i] = momenta[i] + rhs.momenta[i];
    }

    return r;
  }

  scalar simple_y_base::subtract(const y_base& y) const {
    scalar err = 0;
    const simple_y_base& rhs = dynamic_cast<const simple_y_base&>(y);

    for (unsigned int i = 0; i < momenta.size(); ++i) {
      err = std::max(err, convert<scalar>(norm(momenta[i] - rhs.momenta[i])));
    }

    return err;
  }

  f_value simple_body::f() {
    simple_f_base* r = new simple_f_base;

    apply_forces(*this);
    r->forces.resize(size());
    iterator j = begin();
    for (unsigned int i = 0; i < size(); ++i, ++j) {
      r->forces[i] = j->F();
    }

    return f_value(r);
  }

  y_value simple_body::y() {
    simple_y_base* r = new simple_y_base;

    r->t = 0;
    r->momenta.resize(size());
    iterator j = begin();
    for (unsigned int i = 0; i < size(); ++i, ++j) {
      iterator k = r->backup.insert(new particle());
      k->m(j->m());
      k->s(j->s());
      k->v(j->v());
      r->momenta[i] = 0;
    }

    return y_value(r);
  }

  body& simple_body::operator=(const y_value& y) {
    const simple_y_base& rhs = dynamic_cast<const simple_y_base&>(*y.base());

    iterator j = begin();
    const_iterator k = rhs.backup.begin();
    for (unsigned int i = 0; i < rhs.momenta.size(); ++i, ++j, ++k) {
      j->s(k->s() + rhs.t*(k->v() + rhs.momenta[i]/j->m()/2));
      j->v(k->v() + rhs.momenta[i]/j->m());
    }

    return *this;
  }
}
