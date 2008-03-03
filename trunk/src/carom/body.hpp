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

#ifndef CAROM_BODY_HPP
#define CAROM_BODY_HPP

#include <boost/utility.hpp> // For noncopyable
#include <tr1/memory> // For shared_ptr

namespace carom
{
  // Forward declarations
  class body;
  class k_base;

  class f_base
  {
  public:
    // f_base();
    // f_base(const f_base& k);
    virtual ~f_base();

    // f_base& operator=(const f_base& k);

    virtual k_base* multiply(const scalar_time& t) const = 0;
  };

  class k_base
  {
  public:
    // k_base();
    // k_base(const k_base& k);
    virtual ~k_base();

    // k_base& operator=(const k_base& k);

    virtual k_base* add     (const k_base& k) const = 0;
    virtual k_base* subtract(const k_base& k) const = 0;
    virtual k_base* multiply(const scalar& n) const = 0;
    virtual k_base* divide  (const scalar& n) const = 0;
  };

  class y_base
  {
  public:
    typedef polymorphic_list<particle>::iterator       iterator;
    typedef polymorphic_list<particle>::const_iterator const_iterator;

    // y_base();
    // y_base(const y_base& y);
    virtual ~y_base();

    // y_base& operator=(const y_base& y);

    body*       backup();
    const body* backup() const;
    void backup(body* backup);

    virtual scalar subtract(const y_base& y) const;

  private:
    std::tr1::shared_ptr<body> m_backup;
  };

  class f_value
  {
  public:
    f_value();
    explicit f_value(f_base* f);
    // f_value(const f_value& k);
    // ~f_value();

    f_base*       base();
    const f_base* base() const;

  private:
    std::tr1::shared_ptr<f_base> m_base;
  };

  class k_value
  {
  public:
    k_value();
    explicit k_value(k_base* k);
    // k_value(const k_value& k);
    // ~k_value();

    k_base*       base();
    const k_base* base() const;

    // k_value& operator=(const k_value& k);
    k_value& operator+=(const k_value& k);
    k_value& operator-=(const k_value& k);
    k_value& operator*=(const scalar& n);
    k_value& operator/=(const scalar& n);

  private:
    std::tr1::shared_ptr<k_base> m_base;
  };

  class y_value
  {
  public:
    y_value();
    explicit y_value(y_base* y);
    // y_value(const y_value& y);
    // ~y_value();

    y_base*       base();
    const y_base* base() const;

    // y_value& operator=(const y_value& y);

  private:
    std::tr1::shared_ptr<y_base> m_base;
  };

  class body : private boost::noncopyable
  {
  public:
    typedef polymorphic_list<particle>::iterator       iterator;
    typedef polymorphic_list<particle>::const_iterator const_iterator;

    // body();
    virtual ~body();

    iterator insert(particle* x);
    void erase(iterator i);

    iterator       begin();
    const_iterator begin() const;
    iterator       end();
    const_iterator end()   const;

    std::size_t size() const;

    scalar_mass         mass          () const;
    vector_displacement center_of_mass() const;
    vector_velocity     velocity      () const;
    vector_momentum     momentum      () const;
    vector_acceleration acceleration  () const;
    vector_force        force         () const;

    // Needed for collision response
    virtual scalar_mass mass(const particle& x) const = 0;
    virtual void collision(particle& x, const vector_momentum& dp) = 0;

    void apply_forces();

    virtual f_value f() = 0;
    virtual y_value y() = 0;
    virtual void step(const y_value& y0, const k_value& k) = 0;
    virtual void apply(const y_value& y);

  private:
    polymorphic_list<particle> m_particles;
  };

  k_value operator*(const scalar_time& lhs, const f_value&     rhs);
  k_value operator*(const f_value&     lhs, const scalar_time& rhs);
  k_value operator+(const k_value&     lhs, const k_value&     rhs);
  k_value operator-(const k_value&     lhs, const k_value&     rhs);
  k_value operator*(const scalar&      lhs, const k_value&     rhs);
  k_value operator*(const k_value&     lhs, const scalar&      rhs);
  k_value operator/(const k_value&     lhs, const scalar&      rhs);
  scalar  operator-(const y_value&     lhs, const y_value&     rhs);
}

#endif // CAROM_BODY_HPP
