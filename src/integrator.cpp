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
#include <boost/utility.hpp> // For next()/prior()
#include <algorithm> // For max()
#include <vector>

namespace carom
{
  integrator::integrator(system& sys)
    : m_sys(&sys), m_f1(sys.size()), m_y(sys.size()) {
    m_sys->collision();
    system::iterator j = m_sys->begin();
    for (unsigned int i = 0; i < m_sys->size(); ++i, ++j) {
      m_f1[i] = j->f();
      m_y[i] = j->y();
    }
  }

  integrator::~integrator() { }

  scalar_time integrator::integrate(const scalar_time& t,
                                    const scalar_time& dt) {
    scalar_time elapsed = 0, delta = dt;

    while (delta <= t - elapsed) {
      delta = step(delta, elapsed);
    }
    while (elapsed < t) {
      step(t - elapsed, elapsed);
    }

    return delta;
  }

  integrator::k_vector integrator::k(const integrator::a_vector& a_vecs,
                                     const scalar_time& dt) {
    k_vector k_vecs(m_sys->size());

    unsigned int n = 1;
    if (!a_vecs.empty()) {
      // The number of k-values is equal to the number of entries in the last
      // row of the a-value matrix plus one
      n = a_vecs.back().size() + 1;
    }

    // Find k1 using m_f1
    for (unsigned int i = 0; i < k_vecs.size(); ++i) {
      k_vecs[i].resize(n);
      k_vecs[i][0] = dt*m_f1[i];
    }

    // Find k2..n
    for (unsigned int i = 1; i < n; ++i) {
      system::iterator b = m_sys->begin();
      for (unsigned int j = 0; j < m_sys->size(); ++j, ++b) {
        k_value k = a_vecs[i-1][0]*k_vecs[j][0];
        for (unsigned int l = 1; l < i; ++l) {
          k += a_vecs[i-1][l]*k_vecs[j][l];
        }
        b->step(m_y[j], k);
      }

      b = m_sys->begin();
      for (unsigned int j = 0; j < k_vecs.size(); ++j, ++b) {
        k_vecs[j][i] = dt*b->f();
      }
    }

    return k_vecs;
  }

  integrator::y_vector integrator::y(const integrator::b_vector& b_vec,
                                     const integrator::k_vector& k_vecs) {
    y_vector y_vec(k_vecs.size());

    system::iterator b = m_sys->begin();
    for (unsigned int i = 0; i < m_sys->size(); ++i, ++b) {
      k_value k = b_vec[0]*k_vecs[i][0];
      for (unsigned int j = 1; j < b_vec.size(); ++j) {
        k += b_vec[j]*k_vecs[i][j];
      }
      b->step(m_y[i], k);
    }
    m_sys->collision();
    b = m_sys->begin();
    for (unsigned int i = 0; i < m_sys->size(); ++i, ++b) {
      y_vec[i] = b->y();
    }

    return y_vec;
  }

  void integrator::apply(const y_vector& y_vec) {
    system::iterator j = m_sys->begin();
    for (unsigned int i = 0; i < m_sys->size(); ++i, ++j) {
      j->apply(y_vec[i]);
    }
    j = m_sys->begin();
    for (unsigned int i = 0; i < m_sys->size(); ++i, ++j) {
      m_f1[i] = j->f();
      m_y[i] = j->y();
    }
  }

  simple_integrator::simple_integrator(system& sys) : integrator(sys) { }
  simple_integrator::~simple_integrator() { }

  scalar_time
  simple_integrator::simple_step(const scalar_time& dt, scalar_time& elapsed,
                                 const integrator::a_vector& a_vecs,
                                 const integrator::b_vector& b_vec) {
    apply(y(b_vec, k(a_vecs, dt)));
    elapsed += dt;
    return dt;
  }

  adaptive_integrator::adaptive_integrator(system& sys, const scalar& tol,
                                           unsigned int order)
    : integrator(sys), m_tol(tol), m_order(order), m_err(0), m_steps(0) { }
  adaptive_integrator::~adaptive_integrator() { }

  scalar_time
  adaptive_integrator::adaptive_step(const scalar_time& dt,
                                     scalar_time& elapsed,
                                     const integrator::a_vector& a_vecs,
                                     const integrator::b_vector& b_vec,
                                     const integrator::b_vector& bstar_vec) {
    scalar_time delta, deltaprime = dt;
    scalar err;

    bool rejected = true;

    y_vector y_vec;

    while (rejected) {
      k_vector k_vecs = k(a_vecs, deltaprime);
      y_vec = y(b_vec, k_vecs);
      y_vector ystar_vec = y(bstar_vec, k_vecs);

      // Find the error: the maximum error of any body, where the error of a
      // body is the difference between the y_value's of the real and embeded
      // steps
      err = 0;
      for (unsigned int i = 0; i < y_vec.size(); ++i) {
        err = std::max(err, y_vec[i] - ystar_vec[i]);
      }

      // Store the stepsize used for the integration
      delta = deltaprime;

      if (m_steps >= 4) {
        // Find t', the t value that we estimate would have given an error of
        // err/(1 + tol). If the step is rejected, this is our new stepsize;
        // otherwise, this is our recommended stepsize for the next iteration.
        deltaprime *= pow((m_err/m_steps)/(err*(1 + m_tol)),
                          scalar(1)/(m_order + 1));

        if (err > (1 + m_tol)*m_err/m_steps) {
          rejected = true;
        } else {
          rejected = false;
        }
      } else {
        rejected = false;
      }
    }

    m_err += err;
    ++m_steps;

    elapsed += delta;
    apply(y_vec);

    return deltaprime;
  }

  Euler_integrator::Euler_integrator(system& sys) : simple_integrator(sys) { }
  Euler_integrator::~Euler_integrator() { }

  scalar_time Euler_integrator::step(const scalar_time& dt,
                                     scalar_time& elapsed) {
    // Euler method. Simplest Runge-Kutta method. First order. Its tableau is:
    //
    //   0|
    //   -+-
    //    |1
    //
    // y[n + 1] = y[n] + dt*f(y[n])

    scalar b[1] = { 1 };

    a_vector a_vecs;
    b_vector b_vec(b, b + 1);

    return simple_step(dt, elapsed, a_vecs, b_vec);
  }

  midpoint_integrator::midpoint_integrator(system& sys)
    : simple_integrator(sys) { }
  midpoint_integrator::~midpoint_integrator() { }

  scalar_time midpoint_integrator::step(const scalar_time& dt,
                                        scalar_time& elapsed) {
    // Midpoint method. Second order Runge-Kutta method. Requires only two
    // function evaluations per step. Its tableau is:
    //
    //   0  |
    //   1/2|1/2
    //   ---+-----
    //      |0   1
    //
    //    k1    = dt*f(y[n])
    //    k2    = dt*f(y[n] + (dt/2)*k1)
    // y[n + 1] = y[n] + k2

    scalar a2[1] = { scalar(1)/2 };
    scalar b[2] = { 0, 1 };

    std::vector<scalar> a_vec_arr[1] = { std::vector<scalar>(a2, a2 + 1) };
    a_vector a_vecs(a_vec_arr, a_vec_arr + 1);
    b_vector b_vec(b, b + 2);

    return simple_step(dt, elapsed, a_vecs, b_vec);
  }

  RK4_integrator::RK4_integrator(system& sys) : simple_integrator(sys) { }
  RK4_integrator::~RK4_integrator() { }

  scalar_time RK4_integrator::step(const scalar_time& dt,
                                   scalar_time& elapsed) {
    // Classical, fourth order Runge-Kutta method. Requires four function
    // evaluations per step. Its tableau is:
    //
    //   0  |
    //   1/2|1/2
    //   1/2|0   1/2
    //   1  |0   0   1
    //   ---+---------------
    //      |1/6 1/3 1/3 1/6
    //
    //    k1    = dt*f(y[n])
    //    k2    = dt*f(y[n] + (dt/2)*k1)
    //    k3    = dt*f(y[n] + (dt/2)*k2)
    //    k4    = dt*f(y[n] + dt*k3)
    // y[n + 1] = y[n] + (k1 + 2*k2 + 2*k3 + k4)/6

    scalar a2[1] = { scalar(1)/2 };
    scalar a3[2] = { 0          , scalar(1)/2 };
    scalar a4[3] = { 0          , 0          , 1 };
    scalar b[4] = { scalar(1)/6, scalar(1)/3, scalar(1)/3, scalar(1)/6 };

    std::vector<scalar> a_vec_arr[3] = { std::vector<scalar>(a2, a2 + 1),
                                         std::vector<scalar>(a3, a3 + 2),
                                         std::vector<scalar>(a4, a4 + 3) };
    a_vector a_vecs(a_vec_arr, a_vec_arr + 3);
    b_vector b_vec(b, b + 4);

    return simple_step(dt, elapsed, a_vecs, b_vec);
  }

  RKF45_integrator::RKF45_integrator(system& sys, const scalar& tol)
    : adaptive_integrator(sys, tol, 4) { }
  RKF45_integrator::~RKF45_integrator() { }

  scalar_time RKF45_integrator::step(const scalar_time& dt,
                                     scalar_time& elapsed) {
    // Fifth-order Runge-Kutta-Fehlberg adaptive method. Requires 6 function
    // evaluations per step. An embeded fourth-order method gives an estimate
    // of the local truncation error, which is used to either reject the step
    // or find a recommendation for the next stepsize.

    scalar a2[1] = { scalar(1)/4 };
    scalar a3[2] = { scalar(3)/32, scalar(9)/32 };
    scalar a4[3] = { scalar(1932)/2197, -scalar(7200)/2197, scalar(7296)/2197 };
    scalar a5[4] = { scalar(439)/216, -8, scalar(3680)/513,
                     -scalar(845)/4104 };
    scalar a6[5] = { -scalar(8)/27, 2, -scalar(3544)/2565, scalar(1859)/4104,
                     -scalar(11)/40 };

    scalar b[6] = { scalar(16)/135, 0, scalar(6656)/12825, scalar(28561)/56430,
                    -scalar(9)/50, scalar(2)/55 };

    scalar bstar[6] = { scalar(25)/216, 0, scalar(1408)/2565, scalar(2197)/4104,
                        -scalar(1)/5, 0 };

    std::vector<scalar> a_vec_arr[6] = { std::vector<scalar>(a2, a2 + 1),
                                         std::vector<scalar>(a3, a3 + 2),
                                         std::vector<scalar>(a4, a4 + 3),
                                         std::vector<scalar>(a5, a5 + 4),
                                         std::vector<scalar>(a6, a6 + 5) };
    a_vector a_vecs(a_vec_arr, a_vec_arr + 5);
    b_vector b_vec(b, b + 6);
    b_vector bstar_vec(bstar, bstar + 6);

    return adaptive_step(dt, elapsed, a_vecs, b_vec, bstar_vec);
  }

  DP45_integrator::DP45_integrator(system& sys, const scalar& tol)
    : adaptive_integrator(sys, tol, 4) { }
  DP45_integrator::~DP45_integrator() { }

  scalar_time DP45_integrator::step(const scalar_time& dt,
                                     scalar_time& elapsed) {
    // Fifth-order Dormand-Prince adaptive method. Requires 7 function
    // evaluations, rather than 6, because the FSAL optimization hasn't been
    // applied in this case.

    scalar a2[1] = { scalar(1)/5 };
    scalar a3[2] = { scalar(3)/40, scalar(9)/40 };
    scalar a4[3] = { scalar(44)/45, -scalar(56)/15, scalar(32)/9 };
    scalar a5[4] = { scalar(19372)/6561, -scalar(25360)/2187,
                     scalar(64448)/6561, -scalar(212)/729 };
    scalar a6[5] = { scalar(9017)/3168, -scalar(355)/33, scalar(46732)/5247,
                     scalar(49)/176, -scalar(5103)/18656 };
    scalar a7[6] = { scalar(35)/384, 0, scalar(500)/1113, scalar(125)/192,
                     -scalar(2187)/6784, scalar(11)/84 };

    scalar b[7] = { scalar(35)/384, 0, scalar(500)/1113, scalar(125)/192,
                    -scalar(2187)/6784, scalar(11)/84, 0 };

    scalar bstar[7] = { scalar(5179)/576000, 0, scalar(7571)/16695,
                        scalar(393)/640, -scalar(92097)/339200,
                        scalar(187)/2100, scalar(1)/40 };

    std::vector<scalar> a_vec_arr[6] = { std::vector<scalar>(a2, a2 + 1),
                                         std::vector<scalar>(a3, a3 + 2),
                                         std::vector<scalar>(a4, a4 + 3),
                                         std::vector<scalar>(a5, a5 + 4),
                                         std::vector<scalar>(a6, a6 + 5),
                                         std::vector<scalar>(a7, a7 + 6) };
    a_vector a_vecs(a_vec_arr, a_vec_arr + 6);
    b_vector b_vec(b, b + 7);
    b_vector bstar_vec(bstar, bstar + 7);

    return adaptive_step(dt, elapsed, a_vecs, b_vec, bstar_vec);
  }
}
