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
  void system::integrate_Euler(const scalar_time& t) {
    // Euler method. Simplest Runge-Kutta method. First order. Its tableau is:
    //
    //   0|
    //   -+-
    //    |1
    //
    // y[n + 1] = y[n] + t*f(y[n])

    std::vector<k_value> k_vec(size());

    std::vector<k_value>::iterator k;
    iterator i;

    for (i = begin(); i != end(); ++i) {
      i->begin_integration();
    }

    for (i = begin(), k = k_vec.begin(); i != end(); ++i, ++k) {
      *k = i->k();
    }

    for (i = begin(), k = k_vec.begin(); i != end(); ++i, ++k) {
      i->step(*k, t);
    }

    for (i = begin(); i != end(); ++i) {
      i->end_integration();
    }
  }

  void system::integrate_midpoint(const scalar_time& t) {
    // Midpoint method. Second order Runge-Kutta method. Requires only two
    // function evaluations per step. Its tableau is:
    //
    //   0  |
    //   1/2|1/2
    //   ---+-----
    //      |0   1
    //
    //    k1    = f(y[n])
    //    k2    = f(y[n] + (t/2)*k1)
    // y[n + 1] = y[n] + t*k2

    std::vector<k_value> k1_vec(size());
    std::vector<k_value> k2_vec(size());

    std::vector<k_value>::iterator k1, k2;
    iterator i;

    for (i = begin(); i != end(); ++i) {
      i->begin_integration();
    }

    for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
      *k1 = i->k();
    }

    for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
      i->step(*k1, t/2);
    }

    for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
      *k2 = i->k();
    }

    for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
      i->step(*k2, t);
    }

    for (i = begin(); i != end(); ++i) {
      i->end_integration();
    }
  }

  void system::integrate_RK4(const scalar_time& t) {
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
    //    k1    = f(y[n])
    //    k2    = f(y[n] + (t/2)*k1)
    //    k3    = f(y[n] + (t/2)*k2)
    //    k4    = f(y[n] + t*k3)
    // y[n + 1] = y[n] + t*(k1 + 2*k2 + 2*k3 + k4)/6

    std::vector<k_value> k1_vec(size());
    std::vector<k_value> k2_vec(size());
    std::vector<k_value> k3_vec(size());
    std::vector<k_value> k4_vec(size());

    std::vector<k_value>::iterator k1, k2, k3, k4;
    iterator i;

    for (i = begin(); i != end(); ++i) {
      i->begin_integration();
    }

    for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
      *k1 = i->k();
    }

    for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
      i->step(*k1, t/2);
    }

    for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
      *k2 = i->k();
    }

    for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
      i->step(*k2, t/2);
    }

    for (i = begin(), k3 = k3_vec.begin(); i != end(); ++i, ++k3) {
      *k3 = i->k();
    }

    for (i = begin(), k3 = k3_vec.begin(); i != end(); ++i, ++k3) {
      i->step(*k3, t);
    }

    for (i = begin(), k4 = k4_vec.begin(); i != end(); ++i, ++k4) {
      *k4 = i->k();
    }

    for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin(),
                      k3 = k3_vec.begin(), k4 = k4_vec.begin();
         i != end();
         ++i, ++k1, ++k2, ++k3, ++k4) {
      k_value k = (*k1 + 2*(*k2) + 2*(*k3) + *k4)/6;
      i->step(k, t);
    }

    for (i = begin(); i != end(); ++i) {
      i->end_integration();
    }
  }

  scalar_time system::integrate_DP(const scalar_time& t, scalar_time& elapsed,
                                   const scalar& tol) {
    // Fifth-order Dormand-Prince method, with embeded fourth-order formula
    // for adaptive stepsize control. Confer to Numerical Recipies S17.2 for an
    // overview. In this implementation, an average error is stored. If the
    // current step's error is greater than the average by a factor of 1 + tol,
    // we reject the step, find a new estimate for the stepsize, and try again.
    // This routine also increments elapsed by the stepsize used, and returns a
    // suggestion for the next stepsize. This routine also calls collision() in
    // the right places, so that the stepsize adjusts to the error in collision
    // handling as well. This can be quite expensive.

    scalar a21 = scalar(1)/5;
    scalar a31 = scalar(3)/40, a32 = scalar(9)/40;
    scalar a41 = scalar(44)/45, a42 = -scalar(56)/15, a43 = scalar(32)/9;
    scalar a51 = scalar(19372)/6561, a52 = -scalar(25360)/2187,
      a53 = scalar(64448)/6561, a54 = -scalar(212)/729;
    scalar a61 = scalar(9017)/3168, a62 = -scalar(355)/33,
      a63 = scalar(46732)/5247, a64 = scalar(49)/176, a65 = -scalar(5103)/18656;

    scalar b1 = scalar(35)/384, b2 = 0, b3 = scalar(500)/1113,
      b4 = scalar(125)/192, b5 = -scalar(2187)/6784, b6 = scalar(11)/84;

    // b*'s
    scalar bs1 = scalar(5179)/576000, bs2 = 0, bs3 = scalar(7571)/16695,
      bs4 = scalar(393)/640, bs5 = -scalar(92097)/339200,
      bs6 = scalar(187)/2100, bs7 = scalar(1)/40;

    scalar c2 = scalar(1)/5, c3 = scalar(3)/10, c4 = scalar(4)/5,
      c5 = scalar(8)/9, c6 = 1, c7 = 1;

    bool rejected = true;
    scalar_time tprime = t, dt;
    scalar err;

    std::vector<k_value> k1_vec(size());
    std::vector<k_value> k2_vec(size());
    std::vector<k_value> k3_vec(size());
    std::vector<k_value> k4_vec(size());
    std::vector<k_value> k5_vec(size());
    std::vector<k_value> k6_vec(size());

    std::vector<k_value>::iterator k1, k2, k3, k4, k5, k6, k7;
    iterator i;

    for (i = begin(); i != end(); ++i) {
      i->begin_integration();
    }

    // Can we use FSAL?
    if (DP_steps() > 0 && elapsed == m_DP_last_elapsed) {
      k1_vec = m_DP_k7_vec;
    } else {
      for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
        *k1 = i->k();
      }
    }

    while (rejected) {
      // Calculate k2..6

      for (i = begin(), k1 = k1_vec.begin(); i != end(); ++i, ++k1) {
        i->step(a21*(*k1), c2*t);
      }

      for (i = begin(), k2 = k2_vec.begin(); i != end(); ++i, ++k2) {
        *k2 = i->k();
      }

      for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin();
           i != end();
           ++i, ++k1, ++k2) {
        i->step(a31*(*k1) + a32*(*k2), c3*t);
      }

      for (i = begin(), k3 = k3_vec.begin(); i != end(); ++i, ++k3) {
        *k3 = i->k();
      }

      for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin(),
             k3 = k3_vec.begin();
           i != end();
           ++i, ++k1, ++k2, ++k3) {
        i->step(a41*(*k1) + a42*(*k2) + a43*(*k3), c4*t);
      }

      for (i = begin(), k4 = k4_vec.begin(); i != end(); ++i, ++k4) {
        *k4 = i->k();
      }

      for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin(),
             k3 = k3_vec.begin(), k4 = k4_vec.begin();
           i != end();
           ++i, ++k1, ++k2, ++k3, ++k4) {
        i->step(a51*(*k1) + a52*(*k2) + a53*(*k3) + a54*(*k4), c5*t);
      }

      for (i = begin(), k5 = k5_vec.begin(); i != end(); ++i, ++k5) {
        *k5 = i->k();
      }

      for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin(),
             k3 = k3_vec.begin(), k4 = k4_vec.begin(), k5 = k5_vec.begin();
           i != end();
           ++i, ++k1, ++k2, ++k3, ++k4, ++k5) {
        i->step(a61*(*k1) + a62*(*k2) + a63*(*k3) + a64*(*k4) + a65*(*k5),
                c6*t);
      }

      for (i = begin(), k6 = k6_vec.begin(); i != end(); ++i, ++k6) {
        *k6 = i->k();
      }

      // Apply the fifth-order step
      for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin(),
             k3 = k3_vec.begin(), k4 = k4_vec.begin(), k5 = k5_vec.begin(),
             k6 = k6_vec.begin();
           i != end();
           ++i, ++k1, ++k2, ++k3, ++k4, ++k5, ++k6) {
        i->step(b1*(*k1) + b2*(*k2) + b3*(*k3) + b4*(*k4) + b5*(*k5) +
                b6*(*k6), t);
      }

      // Calculate k7, and store it for FSAL
      m_DP_k7_vec.resize(size());
      for (i = begin(), k7 = m_DP_k7_vec.begin(); i != end(); ++i, ++k7) {
        *k7 = i->k();
      }

      // Store the y_value's of the fifth-order step

      std::vector<y_value> y_vec(size());
      std::vector<y_value>::iterator y;

      for (i = begin(), y = y_vec.begin(); i != end(); ++i, ++y) {
        *y = i->y();
      }

      // Apply the fourth-order step
      for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin(),
             k3 = k3_vec.begin(), k4 = k4_vec.begin(), k5 = k5_vec.begin(),
             k6 = k6_vec.begin(), k7 = m_DP_k7_vec.begin();
           i != end();
           ++i, ++k1, ++k2, ++k3, ++k4, ++k5, ++k6) {
        i->step(bs1*(*k1) + bs2*(*k2) + bs3*(*k3) + bs4*(*k4) + bs5*(*k5) +
                bs6*(*k6) + bs7*(*k7), t);
      }

      // Find the error: the maximum error of any body, where the error of a
      // body is the difference between the y_value's of the fifth- and fourth-
      // order steps.
      err = 0;
      for (i = begin(), y = y_vec.begin(); i != end(); ++i, ++y) {
        err = std::max(err, i->y() - *y);
      }

      // Store the stepsize used for the integration
      dt = tprime;

      if (DP_steps() > 0) {
        // Find t', the t value that we estimate would have given an error of
        // DP_average_error()/(1 + tol). The formula has a power of 1/5, because
        // the error calculated is that of the fourth-order step. If the step
        // is rejected, this is our new stepsize; otherwise, this is our
        // recommended stepsize for the next iteration.
        tprime = tprime*pow(DP_average_error()/((1 + tol)*err), scalar(1)/5);

        if (err > (1 + tol)*DP_average_error()) {
          rejected = true;
        } else {
          rejected = false;
        }
      } else {
        rejected = false;
      }
    }

    // Re-apply the fifth-order step

    for (i = begin(), k1 = k1_vec.begin(), k2 = k2_vec.begin(),
           k3 = k3_vec.begin(), k4 = k4_vec.begin(), k5 = k5_vec.begin(),
           k6 = k6_vec.begin();
         i != end();
         ++i, ++k1, ++k2, ++k3, ++k4, ++k5, ++k6) {
      i->step(b1*(*k1) + b2*(*k2) + b3*(*k3) + b4*(*k4) + b5*(*k5) +
              b6*(*k6), t);
    }

    for (i = begin(); i != end(); ++i) {
      i->end_integration();
    }

    elapsed += dt;
    m_DP_last_elapsed = elapsed;
    m_DP_err += err;
    ++m_DP_steps;

    return tprime;
  }

  // Returns the average error per step of DP.
  scalar system::DP_average_error() {
    return DP_accumulated_error()/DP_steps();
  }

  // Returns the accumulated error in the worst-case scenario, where all the
  // errors are assumed to add together with the same sign
  scalar system::DP_accumulated_error() {
    return m_DP_err;
  }

  // The number of steps that have been integrated by DP.
  unsigned long system::DP_steps() {
    return m_DP_steps;
  }

  void system::collision() {
    for (iterator i = begin(); i != end(); ++i) {
      for (iterator j = boost::next(i); j != end(); ++j) {
        carom::collision(*i, *j); // collision() is hidden by system::collision
      }
    }
  }
}
