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

#ifndef CAROM_UTILITY_RIGID_BODY_HPP
#define CAROM_UTILITY_RIGID_BODY_HPP

namespace carom
{
  class rigid_body : public body
  {
  public:
    // rigid_body();
    // virtual ~rigid_body();

    virtual void calculate_k1();
    virtual void calculate_k2();
    virtual void calculate_k3();
    virtual void calculate_k4();

    virtual void apply_k1(const scalar_time& t);
    virtual void apply_k2(const scalar_time& t);
    virtual void apply_k3(const scalar_time& t);

    virtual void apply(const scalar_time& t);

  private:
    scalar_mass              m_m;
    scalar_moment_of_inertia m_I;
    vector_displacement      m_o;
    vector_displacement      m_s;
    vector_angle             m_theta;
    vector_momentum          m_p;
    vector_angular_momentum  m_L;
    vector_force             m_F1, m_F2, m_F3, m_F4;
    vector_torque            m_T1, m_T2, m_T3, m_T4;

    void advance(const scalar_time& t, const vector_force& F,
                 const vector_torque& T);
    void retreat();
  };
}

#endif // CAROM_UTILITY_RIGID_BODY_HPP
