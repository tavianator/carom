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

#ifndef CAROM_SIMPLE_BODY_HPP
#define CAROM_SIMPLE_BODY_HPP

#include <vector>
#include <tr1/memory> // For tr1::shared_ptr

namespace carom
{
  // Forward declaration
  class simple_body;

  class simple_f_base : public f_base
  {
  public:
    simple_f_base(std::size_t n);
    // simple_f_base(const simple_f_base& k);
    virtual ~simple_f_base();

    // simple_f_base& operator=(const simple_f_base& k);

    vector_force&       operator[](unsigned int i);
    const vector_force& operator[](unsigned int i) const;

    void resize(std::size_t n);
    std::size_t size() const;

    virtual k_base* multiply(const scalar_time& t) const;

  private:
    std::vector<vector_force> m_forces;
  };

  class simple_k_base : public k_base
  {
  public:
    simple_k_base(const scalar_time& dt, std::size_t n);
    // simple_k_base(const simple_k_base& k);
    virtual ~simple_k_base();

    // simple_k_base& operator=(const simple_k_base& k);

    scalar_time dt() const;
    void dt(const scalar_time& dt);

    vector_momentum&       operator[](unsigned int i);
    const vector_momentum& operator[](unsigned int i) const;

    void resize(std::size_t n);
    std::size_t size() const;

    virtual k_base* add     (const k_base& k) const;
    virtual k_base* subtract(const k_base& k) const;
    virtual k_base* multiply(const scalar& n) const;
    virtual k_base* divide  (const scalar& n) const;

  private:
    scalar_time m_dt;
    std::vector<vector_momentum> m_momenta;
  };

  class simple_body : public body
  {
  public:
    // simple_body();
    // virtual ~simple_body();

    using body::mass;
    virtual scalar_mass mass(const particle& x) const;
    virtual void collision(particle& x, const vector_momentum& dp);

    virtual f_value f();
    virtual y_value y();
    virtual void step(const y_value& y0, const k_value& k);
  };
}

#endif // CAROM_SIMPLE_BODY_HPP
