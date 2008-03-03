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
    rigid_f_base(const vector_force& F, const vector_torque& T);
    // rigid_f_base(const rigid_f_base& k);
    virtual ~rigid_f_base();

    // rigid_f_base& operator=(const rigid_f_base& k);

    vector_force  F() const;
    vector_torque T() const;
    void F(const vector_force& F);
    void T(const vector_torque& T);

    virtual k_base* multiply(const scalar_time& t) const;

  private:
    vector_force  m_F;
    vector_torque m_T;
  };

  struct rigid_k_base : public k_base
  {
  public:
    rigid_k_base(const scalar_time& dt,
                 const vector_momentum& p, const vector_angular_momentum& L);
    // rigid_k_base(const rigid_k_base& k);
    virtual ~rigid_k_base();

    // rigid_k_base& operator=(const rigid_k_base& k);

    scalar_time             dt() const;
    vector_momentum         dp() const;
    vector_angular_momentum dL() const;
    void dt(const scalar_time& dt);
    void dp(const vector_momentum& dp);
    void dL(const vector_angular_momentum& dL);

    virtual k_base* add     (const k_base& k) const;
    virtual k_base* subtract(const k_base& k) const;
    virtual k_base* multiply(const scalar& n) const;
    virtual k_base* divide  (const scalar& n) const;

  private:
    scalar_time             m_dt;
    vector_momentum         m_dp;
    vector_angular_momentum m_dL;
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

    using body::mass;
    virtual scalar_mass mass(const particle& x) const;
    virtual void collision(particle& x, const vector_momentum& dp);

    virtual f_value f();
    virtual y_value y();
    virtual void step(const y_value& y0, const k_value& k);
  };

  template <>
  scalar_mass impenetrable_body<rigid_body>::mass(const triangle& t);

  template <>
  void impenetrable_body<rigid_body>::collision(const triangle& t,
                                                const vector_momentum& dp);
}

#endif // CAROM_RIGID_BODY_HPP
