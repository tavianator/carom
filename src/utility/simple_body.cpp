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

    for (std::list<vector_force>::const_iterator i = forces.begin();
         i != forces.end();
         ++i) {
      r->momenta.push_back(t*(*i));
    }

    return r;
  }

  simple_k_base::~simple_k_base() {
  }

  k_base* simple_k_base::add(const k_base& k) const {
    simple_k_base* r = new simple_k_base;
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);

    for (std::list<vector_momentum>::const_iterator i = momenta.begin(),
           j = rhs.momenta.begin();
         i != momenta.end() && j != rhs.momenta.end();
         ++i, ++j) {
      r->momenta.push_back(*i + *j);
    }

    return r;
  }

  k_base* simple_k_base::subtract(const k_base& k) const {
    simple_k_base* r = new simple_k_base;
    const simple_k_base& rhs = dynamic_cast<const simple_k_base&>(k);

    for (std::list<vector_momentum>::const_iterator i = momenta.begin(),
           j = rhs.momenta.begin();
         i != momenta.end() && j != rhs.momenta.end();
         ++i, ++j) {
      r->momenta.push_back(*i - *j);
    }

    return r;
  }

  k_base* simple_k_base::multiply(const scalar& n) const {
    simple_k_base* r = new simple_k_base;

    for (std::list<vector_momentum>::const_iterator i = momenta.begin();
         i != momenta.end();
         ++i) {
      r->momenta.push_back(n*(*i));
    }

    return r;
  }

  k_base* simple_k_base::divide(const scalar& n) const {
    simple_k_base* r = new simple_k_base;

    for (std::list<vector_momentum>::const_iterator i = momenta.begin();
         i != momenta.end();
         ++i) {
      r->momenta.push_back((*i)/n);
    }

    return r;
  }

  simple_y_base::~simple_y_base() {
  }

  scalar simple_y_base::subtract(const y_base& y) const {
    scalar err = 0;
    const simple_y_base& rhs = dynamic_cast<const simple_y_base&>(y);

    std::list<vector_displacement>::const_iterator s1 = displacements.begin(),
      s2 = rhs.displacements.begin();
    std::list<vector_velocity>::const_iterator v1 = velocities.begin(),
      v2 = rhs.velocities.begin();
    for (;
         s1 != displacements.end() && s2 != rhs.displacements.end() &&
           v1 != velocities.end() && v2 != rhs.velocities.end();
         ++s1, ++s2, ++v1, ++v2) {
      err = std::max(err, convert<scalar>(norm(*s1 - *s2)) +
                     convert<scalar>(norm(*v1 - *v2)));
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
      r->displacements.push_back(i->s());
      r->velocities.push_back(i->v());
    }

    return y_value(r);
  }

  body& simple_body::operator=(const y_value& y) {
    const simple_y_base& rhs = dynamic_cast<const simple_y_base&>(*y.base());

    iterator i;
    std::list<vector_displacement>::const_iterator j;
    std::list<vector_velocity>::const_iterator k;
    for (i = begin(), j = rhs.displacements.begin(), k = rhs.velocities.begin();
         i != end();
         ++i, ++j, ++k) {
      i->s(*j);
      i->v(*k);
    }

    return *this;
  }
}
