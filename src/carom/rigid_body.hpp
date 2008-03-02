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

#ifndef CAROM_RIGID_BODY_HPP
#define CAROM_RIGID_BODY_HPP

#include <vector>

namespace carom
{
  // Forward declaration
  class rigid_body;

  struct rigid_f_base : public f_base
  {
  public:
    // rigid_f_base();
    // rigid_f_base(const rigid_f_base& k);
    virtual ~rigid_f_base();

    // rigid_f_base& operator=(const rigid_f_base& k);

    virtual k_base* multiply(const scalar_time& t) const;

    vector_force  F;
    vector_torque T;
  };

  struct rigid_k_base : public k_base
  {
  public:
    // rigid_k_base();
    // rigid_k_base(const rigid_k_base& k);
    virtual ~rigid_k_base();

    // rigid_k_base& operator=(const rigid_k_base& k);

    virtual k_base* add     (const k_base& k) const;
    virtual k_base* subtract(const k_base& k) const;
    virtual k_base* multiply(const scalar& n) const;
    virtual k_base* divide  (const scalar& n) const;

    scalar_time dt;
    vector_momentum         dp;
    vector_angular_momentum dL;
  };

  struct rigid_y_base : public y_base
  {
  public:
    // rigid_y_base();
    // rigid_y_base(const rigid_y_base& y);
    virtual ~rigid_y_base();

    // rigid_y_base& operator=(const rigid_y_base& y);

    virtual y_base* add     (const k_base& k) const;
    virtual scalar  subtract(const y_base& y) const;

    scalar_time dt;
    vector_momentum         dp;
    vector_angular_momentum dL;
    std::tr1::shared_ptr<rigid_body> backup;
  };

  class rigid_body : public body
  {
  public:
    // rigid_body();
    // virtual ~rigid_body();

    scalar_moment_of_inertia
    moment_of_inertia(const vector_displacement& o, const vector& axis) const;

    vector_angular_velocity
    angular_velocity(const vector_displacement& o, const vector& axis) const;

    vector_angular_momentum
    angular_momentum(const vector_displacement& o) const;

    vector_angular_acceleration
    angular_acceleration(const vector_displacement& o,
                         const vector& axis) const;

    vector_torque torque(const vector_displacement& o) const;

    virtual scalar_mass mass(const particle& x) const;
    using body::mass;

    virtual f_value f();
    virtual y_value y();
    virtual body& operator=(const y_value& y);
  };

  template <>
  scalar_mass impenetrable_body<rigid_body>::mass(const triangle& t);

  template <>
  void impenetrable_body<rigid_body>::collision(const triangle& t,
                                                const vector_momentum& dp);
}

#endif // CAROM_RIGID_BODY_HPP
