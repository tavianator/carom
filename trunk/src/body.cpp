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
#include <tr1/memory> // For shared_ptr

namespace carom
{
  f_base::~f_base() { }
  k_base* f_base::multiply(const scalar_time& t) const { return new k_base(); }

  k_base::~k_base() { }
  k_base* k_base::add     (const k_base& k) const { return new k_base(); }
  k_base* k_base::subtract(const k_base& k) const { return new k_base(); }
  k_base* k_base::multiply(const scalar& n) const { return new k_base(); }
  k_base* k_base::divide  (const scalar& n) const { return new k_base(); }

  y_base::~y_base() { }
  y_base* y_base::add     (const k_base& k) const { return new y_base(); }
  scalar  y_base::subtract(const y_base& y) const { return 0; }

  f_value::f_value() { }
  f_value::f_value(f_base* f) : m_base(f) { }
  f_base*       f_value::base()       { return m_base.get(); }
  const f_base* f_value::base() const { return m_base.get(); }

  k_value::k_value() { }
  k_value::k_value(k_base* k) : m_base(k) { }
  k_base*       k_value::base()       { return m_base.get(); }
  const k_base* k_value::base() const { return m_base.get(); }

  k_value& k_value::operator+=(const k_value& k) {
    m_base.reset(m_base->add(*k.base()));
    return *this;
  }

  k_value& k_value::operator-=(const k_value& k) {
    m_base.reset(m_base->subtract(*k.base()));
    return *this;
  }

  k_value& k_value::operator*=(const scalar& n) {
    m_base.reset(m_base->multiply(n));
    return *this;
  }

  k_value& k_value::operator/=(const scalar& n) {
    m_base.reset(m_base->divide(n));
    return *this;
  }

  y_value::y_value() { }
  y_value::y_value(y_base* y) : m_base(y) { }
  y_base*       y_value::base()       { return m_base.get(); }
  const y_base* y_value::base() const { return m_base.get(); }

  y_value& y_value::operator+=(const k_value& k) {
    m_base.reset(m_base->add(*k.base()));
    return *this;
  }

  body::~body() { }

  body::iterator body::insert(particle* x) {
    return m_particles.insert(m_particles.end(), x);
  }

  void body::erase(body::iterator i) { m_particles.erase(i); }

  body::iterator       body::begin()       { return m_particles.begin(); }
  body::const_iterator body::begin() const { return m_particles.begin(); }
  body::iterator       body::end()         { return m_particles.end(); }
  body::const_iterator body::end()   const { return m_particles.end(); }

  std::size_t body::size() const { return m_particles.size(); }

  scalar_mass body::mass() const {
    scalar_mass m = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      m += i->m();
    }
    return m;
  }

  vector_displacement body::center_of_mass() const {
    vector_units<1, 1, 0> r = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      r += i->m()*i->s();
    }
    return r/mass();
  }

  vector_velocity body::velocity() const {
    return momentum()/mass();
  }

  vector_momentum body::momentum() const {
    vector_momentum p = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      p += i->p();
    }
    return p;
  }

  vector_acceleration body::acceleration() const {
    return force()/mass();
  }

  vector_force body::force() const {
    vector_force F = 0;
    for (const_iterator i = begin(); i != end(); ++i) {
      F += i->F();
    }
    return F;
  }

  void body::apply_forces() {
    for (iterator i = begin(); i != end(); ++i) {
      i->apply_forces();
    }
  }

  f_value body::f() { return f_value(); }
  y_value body::y() { return y_value(); }

  body& body::operator=(const y_value& y) {
    return *this;
  }

  k_value operator*(const scalar_time& lhs, const f_value& rhs) {
    return k_value(rhs.base()->multiply(lhs));
  }

  k_value operator*(const f_value& lhs, const scalar_time& rhs) {
    return k_value(lhs.base()->multiply(rhs));
  }

  k_value operator+(const k_value& lhs, const k_value& rhs) {
    return k_value(lhs.base()->add(*rhs.base()));
  }

  k_value operator-(const k_value& lhs, const k_value& rhs) {
    return k_value(lhs.base()->subtract(*rhs.base()));
  }

  k_value operator*(const scalar& lhs, const k_value& rhs) {
    return k_value(rhs.base()->multiply(lhs));
  }

  k_value operator*(const k_value& lhs, const scalar& rhs) {
    return k_value(lhs.base()->multiply(rhs));
  }

  k_value operator/(const k_value& lhs, const scalar& rhs) {
    return k_value(lhs.base()->divide(rhs));
  }

  y_value operator+(const y_value& lhs, const k_value& rhs) {
    return y_value(lhs.base()->add(*rhs.base()));
  }

  scalar operator-(const y_value& lhs, const y_value& rhs) {
    return lhs.base()->subtract(*rhs.base());
  }
}
