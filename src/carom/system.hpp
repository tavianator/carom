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

#ifndef CAROM_SYSTEM_HPP
#define CAROM_SYSTEM_HPP

#include <boost/utility.hpp> // For noncopyable
#include <vector>

namespace carom
{
  class system : private boost::noncopyable
  {
  public:
    typedef polymorphic_list<body>::iterator       iterator;
    typedef polymorphic_list<body>::const_iterator const_iterator;

    system();
    // ~system();

    iterator insert(body* b) { return m_bodies.insert(m_bodies.end(), b); }
    void erase(iterator i) { m_bodies.erase(i); }

    iterator       begin()       { return m_bodies.begin(); }
    const_iterator begin() const { return m_bodies.begin(); }
    iterator       end()         { return m_bodies.end(); }
    const_iterator end() const   { return m_bodies.end(); }

    std::size_t size() const { return m_bodies.size(); }

    void        integrate_Euler   (const scalar_time& dt);
    void        integrate_midpoint(const scalar_time& dt);
    void        integrate_RK4     (const scalar_time& dt);
    scalar_time integrate_RKF45   (const scalar_time& dt, scalar_time& elapsed,
                                   const scalar& tol);
    scalar_time integrate_DP45    (const scalar_time& dt, scalar_time& elapsed,
                                   const scalar& tol);

    scalar average_error();
    scalar accumulated_error();
    unsigned long steps();
    unsigned long rejected();

    void collision();

  private:
    polymorphic_list<body> m_bodies;
    scalar m_RKF45_err, m_DP45_err;
    unsigned long m_RKF45_steps, m_DP45_steps;
    unsigned long m_rejected;
  };

  class tableau
  {
  public:
    tableau(system& sys);
    // ~tableau();

    std::vector<std::vector<k_value> >
    k(const std::vector<std::vector<scalar> >& a_vecs, const scalar_time& dt);

    std::vector<y_value>
    y(const std::vector<std::vector<k_value> >& k_vecs,
      const std::vector<scalar>& b_vec);

    void apply(const std::vector<y_value>& y_vec);

  private:
    std::vector<f_value> m_f1;
    std::vector<y_value> m_y;
    system* m_sys;
  };
}

#endif // CAROM_SYSTEM_HPP
