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
#include <tr1/memory> // For shared_ptr

namespace carom
{
  f_base::~f_base() {
  }

  k_base* f_base::multiply(const scalar_time& t) const {
    return new k_base();
  }

  k_base::~k_base() {
  }

  k_base* k_base::add(const k_base& k) const {
    return new k_base();
  }

  k_base* k_base::subtract(const k_base& k) const {
    return new k_base();
  }

  k_base* k_base::multiply(const scalar& n) const {
    return new k_base();
  }

  k_base* k_base::divide(const scalar& n) const {
    return new k_base();
  }

  y_base::~y_base() {
  }

  y_base* y_base::add(const k_base& k) const {
    return new y_base;
  }

  scalar y_base::subtract(const y_base& y) const {
    return 0;
  }

  body::~body() {
  }

  f_value body::f() {
    return f_value();
  }

  y_value body::y() {
    return y_value();
  }

  body& body::operator=(const y_value& y) {
    return *this;
  }
}
