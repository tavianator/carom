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
  system::system() : m_err(0), m_steps(0), m_rejected(0) { }

  // All of these functions will look nicer when initialization lists are added
  // to C++

  void system::integrate_Euler(const scalar_time& dt) {
    // Euler method. Simplest Runge-Kutta method. First order. Its tableau is:
    //
    //   0|
    //   -+-
    //    |1
    //
    // y[n + 1] = y[n] + t*f(y[n])

    scalar b[1] = { 1 };

    std::vector<std::vector<scalar> > a_vecs;
    std::vector<scalar> b_vec(b, b + 1);

    tableau tab(*this);
    tab.apply(tab.y(tab.k(a_vecs, dt), b_vec));
  }

  void system::integrate_midpoint(const scalar_time& dt) {
    // Midpoint method. Second order Runge-Kutta method. Requires only two
    // function evaluations per step. Its tableau is:
    //
    //   0  |
    //   1/2|1/2
    //   ---+-----
    //      |0   1
    //
    //    k1    = t*f(y[n])
    //    k2    = t*f(y[n] + (t/2)*k1)
    // y[n + 1] = y[n] + k2

    scalar a2[1] = { scalar(1)/2 };
    scalar b[2] = { 0, 1 };

    std::vector<scalar> a_vec_arr[1] = { std::vector<scalar>(a2, a2 + 1) };
    std::vector<std::vector<scalar> > a_vecs(a_vec_arr, a_vec_arr + 1);
    std::vector<scalar> b_vec(b, b + 2);

    tableau tab(*this);
    tab.apply(tab.y(tab.k(a_vecs, dt), b_vec));
  }

  void system::integrate_RK4(const scalar_time& dt) {
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
    //    k1    = t*f(y[n])
    //    k2    = t*f(y[n] + (t/2)*k1)
    //    k3    = t*f(y[n] + (t/2)*k2)
    //    k4    = t*f(y[n] + t*k3)
    // y[n + 1] = y[n] + (k1 + 2*k2 + 2*k3 + k4)/6

    scalar a2[1] = { scalar(1)/2 };
    scalar a3[2] = { 0          , scalar(1)/2 };
    scalar a4[3] = { 0          , 0          , 1 };
    scalar b[4] = { scalar(1)/6, scalar(1)/3, scalar(1)/3, scalar(1)/6 };

    std::vector<scalar> a_vec_arr[3] = { std::vector<scalar>(a2, a2 + 1),
                                         std::vector<scalar>(a3, a3 + 2),
                                         std::vector<scalar>(a4, a4 + 3) };
    std::vector<std::vector<scalar> > a_vecs(a_vec_arr, a_vec_arr + 3);
    std::vector<scalar> b_vec(b, b + 4);

    tableau tab(*this);
    tab.apply(tab.y(tab.k(a_vecs, dt), b_vec));
  }

  scalar_time system::integrate_RKF45(const scalar_time& dt,
                                      scalar_time& elapsed, const scalar& tol) {
    // Fifth-order Runge-Kutta-Fehlberg adaptive method. Requires 6 function
    // evaluations per step. An embeded fourth-order method gives an estimate
    // of the local truncation error, which is used to either reject the step
    // or find a recommendation for the next stepsize. Steps are rejected if
    // they exceed the average local error by a factor of 1 + tol. This average
    // error is shared between this method and the Dormand-Prince method.

    scalar a2[1] = { scalar(1)/4 };
    scalar a3[2] = { scalar(3)/32, scalar(9)/32 };
    scalar a4[3] = { scalar(1932)/2197, -scalar(7200)/2197, scalar(7296)/2197 };
    scalar a5[4] = { scalar(439)/216, -8, scalar(3680)/513,
                     -scalar(845)/4104 };
    scalar a6[5] = { -scalar(8)/27, 2, -scalar(3544)/2565, scalar(1859)/4104,
                     -scalar(11)/40 };

    scalar b[7] = { scalar(16)/135, 0, scalar(6656)/12825, scalar(29561)/56430,
                    -scalar(9)/50, scalar(2)/55 };

    scalar bstar[7] = { scalar(25)/216, 0, scalar(1408)/2565, scalar(2197)/4104,
                        -scalar(1)/5, 0 };

    std::vector<scalar> a_vec_arr[6] = { std::vector<scalar>(a2, a2 + 1),
                                         std::vector<scalar>(a3, a3 + 2),
                                         std::vector<scalar>(a4, a4 + 3),
                                         std::vector<scalar>(a5, a5 + 4),
                                         std::vector<scalar>(a6, a6 + 5) };
    std::vector<std::vector<scalar> > a_vec(a_vec_arr, a_vec_arr + 5);
    std::vector<scalar> b_vec(b, b + 6);
    std::vector<scalar> bstar_vec(bstar, bstar + 6);

    tableau tab(*this);
    std::vector<y_value> y_vec;

    bool rejected = true;
    scalar_time dtprime = dt, t;
    scalar err;

    while (rejected) {
      // Store the y_value's of the fourth- and fifth-order steps
      std::vector<std::vector<k_value> > k_vecs = tab.k(a_vec, dtprime);
      y_vec = tab.y(k_vecs, b_vec);
      std::vector<y_value> ystar_vec = tab.y(k_vecs, bstar_vec);

      // Find the error: the maximum error of any body, where the error of a
      // body is the difference between the y_value's of the fifth- and fourth-
      // order steps.
      err = 0;
      for (unsigned int i = 0; i < y_vec.size(); ++i) {
        err = std::max(err, y_vec[i] - ystar_vec[i]);
      }

      // Store the stepsize used for the integration
      t = dtprime;

      if (steps() > 0) {
        // Find t', the t value that we estimate would have given an error of
        // average_error()/(1 + tol). The formula has a power of 1/5, because
        // the error calculated is that of the fourth-order step. If the step
        // is rejected, this is our new stepsize; otherwise, this is our
        // recommended stepsize for the next iteration.
        dtprime *= pow(average_error()/err, scalar(1)/5)/(1 + tol);

        if (err > (1 + tol)*average_error()) {
          ++m_rejected;
          rejected = true;
        } else {
          rejected = false;
        }
      } else {
        rejected = false;
      }
    }

    tab.apply(y_vec);
    elapsed += t;
    m_err += err;
    ++m_steps;

    return dtprime;
  }

  scalar_time system::integrate_DP45(const scalar_time& dt,
                                     scalar_time& elapsed, const scalar& tol) {
    // Fifth-order Dormand-Prince adaptive method. Requires 7 function
    // evaluations, rather than 6, because the FSAL optimization can't be
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
    std::vector<std::vector<scalar> > a_vec(a_vec_arr, a_vec_arr + 6);
    std::vector<scalar> b_vec(b, b + 7);
    std::vector<scalar> bstar_vec(bstar, bstar + 7);

    tableau tab(*this);
    std::vector<y_value> y_vec;

    bool rejected = true;
    scalar_time dtprime = dt, t;
    scalar err;

    while (rejected) {
      // Store the y_value's of the fourth- and fifth-order steps
      std::vector<std::vector<k_value> > k_vecs = tab.k(a_vec, dtprime);
      y_vec = tab.y(k_vecs, b_vec);
      std::vector<y_value> ystar_vec = tab.y(k_vecs, bstar_vec);

      // Find the error: the maximum error of any body, where the error of a
      // body is the difference between the y_value's of the fifth- and fourth-
      // order steps.
      err = 0;
      for (unsigned int i = 0; i < y_vec.size(); ++i) {
        err = std::max(err, y_vec[i] - ystar_vec[i]);
      }

      // Store the stepsize used for the integration
      t = dtprime;

      if (steps() > 0) {
        // Find t', the t value that we estimate would have given an error of
        // average_error()/(1 + tol). The formula has a power of 1/5, because
        // the error calculated is that of the fourth-order step. If the step
        // is rejected, this is our new stepsize; otherwise, this is our
        // recommended stepsize for the next iteration.
        dtprime *= pow(average_error()/err, scalar(1)/5)/(1 + tol);

        if (err > (1 + tol)*average_error()) {
          ++m_rejected;
          rejected = true;
        } else {
          rejected = false;
        }
      } else {
        rejected = false;
      }
    }

    tab.apply(y_vec);
    elapsed += t;
    m_err += err;
    ++m_steps;

    return dtprime;
  }

  // Returns the average error per step
  scalar system::average_error() {
    return accumulated_error()/steps();
  }

  // Returns the accumulated error in the worst-case scenario, where all the
  // errors are assumed to add together with the same sign
  scalar system::accumulated_error() {
    return m_err;
  }

  // The number of steps that have been integrated by DP.
  unsigned long system::steps() {
    return m_steps;
  }

  // The number of times we've rejected a step.
  unsigned long system::rejected() {
    return m_rejected;
  }

  void system::collision() {
    for (iterator i = begin(); i != end(); ++i) {
      for (iterator j = boost::next(i); j != end(); ++j) {
        carom::collision(*i, *j); // collision() is hidden by system::collision
      }
    }
  }

  tableau::tableau(system& sys)
    : m_f1(sys.size()), m_y(sys.size()), m_sys(&sys)
  {
    system::iterator j = m_sys->begin();
    for (unsigned int i = 0; i < m_sys->size(); ++i, ++j) {
      m_f1[i] = j->f();
      m_y[i] = j->y();
    }
  }

  std::vector<std::vector<k_value> >
  tableau::k(const std::vector<std::vector<scalar> >& a_vecs,
             const scalar_time& dt) {
    std::vector<std::vector<k_value> > k_vecs(m_sys->size());

    unsigned int n = 1;
    if (!a_vecs.empty()) {
      // The number of k_values is equal to the number of entries in the last
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
      system::iterator k = m_sys->begin();
      for (unsigned int j = 0; j < m_sys->size(); ++j, ++k) {
        y_value y = m_y[j];
        for (unsigned int l = 0; l < i; ++l) {
          y += a_vecs[i-1][l]*k_vecs[j][l];
        }
        *k = y;
      }
      m_sys->collision();

      k = m_sys->begin();
      for (unsigned int j = 0; j < k_vecs.size(); ++j, ++k) {
        k_vecs[j][i] = dt*k->f();
      }
    }

    return k_vecs;
  }

  std::vector<y_value>
  tableau::y(const std::vector<std::vector<k_value> >& k_vecs,
             const std::vector<scalar>& b_vec) {
    std::vector<y_value> y_vec(k_vecs.size());

    for (unsigned int i = 0; i < y_vec.size(); ++i) {
      y_vec[i] = m_y[i];
      for (unsigned int j = 0; j < b_vec.size(); ++j) {
        y_vec[i] += b_vec[j]*k_vecs[i][j];
      }
    }

    return y_vec;
  }

  void tableau::apply(const std::vector<y_value>& y_vec) {
    system::iterator j = m_sys->begin();
    for (unsigned int i = 0; i < m_sys->size(); ++i, ++j) {
      *j = y_vec[i];
      clear_forces(*j);
    }
    m_sys->collision();
  }
}
