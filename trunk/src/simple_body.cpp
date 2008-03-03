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
  simple_f_base::simple_f_base(std::size_t n) : m_forces(n) { }

  simple_f_base::~simple_f_base() {
  }

  vector_force& simple_f_base::operator[](unsigned int i) {
    return m_forces.at(i);
  }

  const vector_force& simple_f_base::operator[](unsigned int i) const {
    return m_forces.at(i);
  }

  void simple_f_base::resize(std::size_t n) { m_forces.resize(n); }
  std::size_t simple_f_base::size() const { return m_forces.size(); }

  k_base* simple_f_base::multiply(const scalar_time& t) const {
    simple_k_base* r = new simple_k_base(t, size());

    for (unsigned int i = 0; i < size(); ++i) {
      (*r)[i] = t*(*this)[i];
    }

    return r;
  }

  simple_k_base::simple_k_base(const scalar_time& dt, std::size_t n)
    : m_dt(dt), m_momenta(n) { }
  simple_k_base::~simple_k_base() {
  }

  scalar_time simple_k_base::dt() const { return m_dt; }
  void simple_k_base::dt(const scalar_time& dt) { m_dt = dt; }

  vector_momentum& simple_k_base::operator[](unsigned int i) {
    return m_momenta.at(i);
  }

  const vector_momentum& simple_k_base::operator[](unsigned int i) const {
    return m_momenta.at(i);
  }

  void simple_k_base::resize(std::size_t n) { m_momenta.resize(n); }
  std::size_t simple_k_base::size() const { return m_momenta.size(); }

  k_base* simple_k_base::add(const k_base& k) const {
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);
    simple_k_base* r = new simple_k_base(dt() + rhs.dt(), size());

    for (unsigned int i = 0; i < size(); ++i) {
      (*r)[i] = (*this)[i] + rhs[i];
    }

    return r;
  }

  k_base* simple_k_base::subtract(const k_base& k) const {
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);
    simple_k_base* r = new simple_k_base(dt() + rhs.dt(), size());

    for (unsigned int i = 0; i < size(); ++i) {
      (*r)[i] = (*this)[i] - rhs[i];
    }

    return r;
  }

  k_base* simple_k_base::multiply(const scalar& n) const {
    simple_k_base* r = new simple_k_base(n*dt(), size());

    for (unsigned int i = 0; i < size(); ++i) {
      (*r)[i] = n*(*this)[i];
    }

    return r;
  }

  k_base* simple_k_base::divide(const scalar& n) const {
    simple_k_base* r = new simple_k_base(dt()/n, size());

    for (unsigned int i = 0; i < size(); ++i) {
      (*r)[i] = (*this)[i]/n;
    }

    return r;
  }

  scalar_mass simple_body::mass(const particle& x) const {
    return x.m();
  }

  void simple_body::collision(particle& x, const vector_momentum& dp) {
    x.p(x.p() + dp);
  }

  f_value simple_body::f() {
    simple_f_base* r = new simple_f_base(size());

    apply_forces();
    iterator j = begin();
    for (unsigned int i = 0; i < size(); ++i, ++j) {
      (*r)[i] = j->F();
    }

    return f_value(r);
  }

  y_value simple_body::y() {
    y_base* r = new y_base();

    r->backup(new simple_body());
    for (iterator i = begin(); i != end(); ++i) {
      iterator j = r->backup()->insert(new particle());
      j->m(i->m());
      j->s(i->s());
      j->p(i->p());
    }

    return y_value(r);
  }

  void simple_body::step(const y_value& y0, const k_value& kv) {
    const simple_k_base& kval = dynamic_cast<const simple_k_base&>(*kv.base());

    iterator i = begin();
    const_iterator j = y0.base()->backup()->begin();
    for (unsigned int k = 0; k < kval.size(); ++i, ++j, ++k) {
      i->s(j->s() + kval.dt()*(j->p() + kval[k]/2)/j->m());
      i->p(j->p() + kval[k]);
    }
  }
}
