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

#ifndef CAROM_UTILITY_SIMPLE_BODY_HPP
#define CAROM_UTILITY_SIMPLE_BODY_HPP

#include <list>

namespace carom
{
  struct simple_f_base : public f_base
  {
  public:
    // simple_f_base();
    // simple_f_base(const simple_f_base& k);
    virtual ~simple_f_base();

    // simple_f_base& operator=(const simple_f_base& k);

    virtual k_base* multiply(const scalar_time& t) const;

    std::list<vector_force> forces;
  };

  struct simple_k_base : public k_base
  {
  public:
    // simple_k_base();
    // simple_k_base(const simple_k_base& k);
    virtual ~simple_k_base();

    // simple_k_base& operator=(const simple_k_base& k);

    virtual k_base* add     (const k_base& k) const;
    virtual k_base* subtract(const k_base& k) const;
    virtual k_base* multiply(const scalar& n) const;
    virtual k_base* divide  (const scalar& n) const;

    scalar_time t;
    std::list<vector_force> forces;
  };

  struct simple_y_base : public y_base
  {
  public:
    // simple_y_base();
    // simple_y_base(const simple_y_base& y);
    virtual ~simple_y_base();

    // simple_y_base& operator=(const simple_y_base& y);

    virtual y_base* add     (const k_base& k) const;
    virtual scalar  subtract(const y_base& y) const;

    body b;
  };

  class simple_body : public body
  {
  public:
    // simple_body();
    // virtual ~simple_body();

    virtual f_value f();
    virtual y_value y();
    virtual body& operator=(const y_value& y);
  };
}

#endif // CAROM_UTILITY_SIMPLE_BODY_HPP
