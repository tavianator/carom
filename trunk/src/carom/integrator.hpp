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

#ifndef CAROM_INTEGRATOR_HPP
#define CAROM_INTEGRATOR_HPP

#include <boost/utility.hpp> // For noncopyable
#include <vector>

namespace carom
{
  class integrator : private boost::noncopyable
  {
  public:
    integrator(system& sys);
    virtual ~integrator();

    void integrate(const scalar_time& t, const scalar_time& dt);

  protected:
    typedef std::vector<std::vector<scalar> > a_vector;
    typedef std::vector<scalar> b_vector;
    typedef std::vector<std::vector<k_value> > k_vector;
    typedef std::vector<y_value> y_vector;

    k_vector k(const a_vector& a_vecs, const scalar_time& dt);
    y_vector y(const b_vector& b_vec, const k_vector& k_vecs);
    void apply(const y_vector& y_vec);

  private:
    system* m_sys;
    std::vector<f_value> m_f1;
    y_vector m_y;

    virtual scalar_time step(const scalar_time& dt, scalar_time& elapsed) = 0;
  };

  class simple_integrator : public integrator
  {
  public:
    simple_integrator(system& sys);
    virtual ~simple_integrator();

  protected:
    scalar_time simple_step(const scalar_time& dt, scalar_time& elapsed,
                            const a_vector& a_vecs, const b_vector& b_vec);
  };

  class adaptive_integrator : public integrator
  {
  public:
    adaptive_integrator(system& sys, const scalar& tol, unsigned int order);
    virtual ~adaptive_integrator();

  protected:
    scalar_time adaptive_step(const scalar_time& dt, scalar_time& elapsed,
                              const a_vector& a_vecs, const b_vector& b_vec,
                              const b_vector& bstar_vec);

  private:
    scalar m_tol;
    unsigned int m_order;
    scalar m_err;
    int m_steps;
  };

  class Euler_integrator : public simple_integrator
  {
  public:
    Euler_integrator(system& sys);
    ~Euler_integrator();

  private:
    virtual scalar_time step(const scalar_time& dt, scalar_time& elapsed);
  };

  class midpoint_integrator : public simple_integrator
  {
  public:
    midpoint_integrator(system& sys);
    ~midpoint_integrator();

  private:
    virtual scalar_time step(const scalar_time& dt, scalar_time& elapsed);
  };

  class RK4_integrator : public simple_integrator
  {
  public:
    RK4_integrator(system& sys);
    ~RK4_integrator();

  private:
    virtual scalar_time step(const scalar_time& dt, scalar_time& elapsed);
  };

  class RKF45_integrator : public adaptive_integrator
  {
  public:
    RKF45_integrator(system& sys, const scalar& tol);
    ~RKF45_integrator();

  private:
    virtual scalar_time step(const scalar_time& dt, scalar_time& elapsed);
  };

  class DP45_integrator : public adaptive_integrator
  {
  public:
    DP45_integrator(system& sys, const scalar& tol);
    ~DP45_integrator();

  private:
    virtual scalar_time step(const scalar_time& dt, scalar_time& elapsed);
  };
}

#endif // CAROM_INTEGRATOR_HPP
