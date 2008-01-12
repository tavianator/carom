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
  struct k_base;

  struct f_base
  {
  public:
    // f_base();
    // f_base(const f_base& k);
    virtual ~f_base();

    // f_base& operator=(const f_base& k);

    virtual k_base* multiply(const scalar_time& t) const;
  };

  struct k_base
  {
  public:
    // k_base();
    // k_base(const k_base& k);
    virtual ~k_base();

    // k_base& operator=(const k_base& k);

    virtual k_base* add     (const k_base& k) const;
    virtual k_base* subtract(const k_base& k) const;
    virtual k_base* multiply(const scalar& n) const;
    virtual k_base* divide  (const scalar& n) const;
  };

  struct y_base
  {
  public:
    // y_base();
    // y_base(const y_base& y);
    virtual ~y_base();

    // y_base& operator=(const y_base& y);

    virtual y_base* add     (const k_base& k) const;
    virtual scalar  subtract(const y_base& y) const;
  };

  class f_value
  {
  public:
    f_value() { }
    explicit f_value(f_base* k) : m_base(k) { }
    // f_value(const f_value& k);
    // ~f_value();

    f_base*       base()       { return m_base.get(); }
    const f_base* base() const { return m_base.get(); }

  private:
    std::tr1::shared_ptr<f_base> m_base;
  };

  class k_value
  {
  public:
    k_value() { }
    explicit k_value(k_base* k) : m_base(k) { }
    // k_value(const k_value& k);
    // ~k_value();

    k_base*       base()       { return m_base.get(); }
    const k_base* base() const { return m_base.get(); }

    // k_value& operator=(const k_value& k);
    k_value& operator+=(const k_value& k)
    { m_base.reset(m_base->add(*k.base())); return *this; }
    k_value& operator-=(const k_value& k)
    { m_base.reset(m_base->subtract(*k.base())); return *this; }
    k_value& operator*=(const scalar& n)
    { m_base.reset(m_base->multiply(n)); return *this; }
    k_value& operator/=(const scalar& n)
    { m_base.reset(m_base->divide(n)); return *this; }

  private:
    std::tr1::shared_ptr<k_base> m_base;
  };

  class y_value
  {
  public:
    y_value() { }
    explicit y_value(y_base* y) : m_base(y) { }
    // y_value(const y_value& y);
    // ~y_value();

    y_base*       base()       { return m_base.get(); }
    const y_base* base() const { return m_base.get(); }

    // y_value& operator=(const y_value& y);
    y_value& operator+=(const k_value& k)
    { m_base.reset(m_base->add(*k.base())); return *this; }

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

    iterator insert(particle* x)
    { return m_particles.insert(m_particles.end(), x); }
    void erase(iterator i) { m_particles.erase(i); }

    iterator       begin()       { return m_particles.begin(); }
    const_iterator begin() const { return m_particles.begin(); }
    iterator       end()         { return m_particles.end(); }
    const_iterator end() const   { return m_particles.end(); }

    std::size_t size() const { return m_particles.size(); }

    virtual f_value f();
    virtual y_value y();
    virtual body& operator=(const y_value& y);

  private:
    polymorphic_list<particle> m_particles;
  };

  inline k_value operator*(const scalar_time& lhs, const f_value& rhs) {
    return k_value(rhs.base()->multiply(lhs));
  }

  inline k_value operator+(const k_value& lhs, const k_value& rhs) {
    return k_value(lhs.base()->add(*rhs.base()));
  }

  inline k_value operator-(const k_value& lhs, const k_value& rhs) {
    return k_value(lhs.base()->subtract(*rhs.base()));
  }

  inline k_value operator*(const scalar& lhs, const k_value& rhs) {
    return k_value(rhs.base()->multiply(lhs));
  }

  inline k_value operator/(const k_value& lhs, const scalar& rhs) {
    return k_value(lhs.base()->divide(rhs));
  }

  inline y_value operator+(const y_value& lhs, const k_value& rhs) {
    return y_value(lhs.base()->add(*rhs.base()));
  }

  inline scalar operator-(const y_value& lhs, const y_value& rhs) {
    return lhs.base()->subtract(*rhs.base());
  }
}

#endif // CAROM_BODY_HPP
