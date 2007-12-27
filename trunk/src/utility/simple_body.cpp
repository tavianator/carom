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
#include <list>

namespace carom
{
  void simple_body::calculate_k1() {
    m_backup = new body();

    for (const_iterator i = begin(); i != end(); ++i) {
      iterator j = m_backup->insert(new particle());
      j->m(i->m());
      j->s(i->s());
      j->p(i->p());
    }

    calculate(m_F1);
  }

  void simple_body::calculate_k2() {
    calculate(m_F2);
  }

  void simple_body::calculate_k3() {
    calculate(m_F3);
  }

  void simple_body::calculate_k4() {
    calculate(m_F4);
  }

  void simple_body::apply_k1(const scalar_time& t) {
    advance(t);
  }

  void simple_body::apply_k2(const scalar_time& t) {
    retreat();
    advance(t);
  }

  void simple_body::apply_k3(const scalar_time& t) {
    retreat();
    advance(t);
  }

  void simple_body::apply(const scalar_time& t) {
    retreat();

    std::list<vector_force>::iterator i = m_F1.begin();
    std::list<vector_force>::iterator j = m_F2.begin();
    std::list<vector_force>::iterator k = m_F3.begin();
    std::list<vector_force>::iterator l = m_F4.begin();
    for (iterator m = begin(); m != end(); ++i, ++j, ++k, ++l, ++m) {
      m->F((*i + 2*(*j) + 2*(*k) + *l)/6);
    }

    advance(t);
    clear_forces(*this);
    m_F1.clear();
    m_F2.clear();
    m_F3.clear();
    m_F4.clear();
    delete m_backup;
  }

  void simple_body::calculate(std::list<vector_force>& F) {
    apply_forces(*this);
    for (iterator i = begin(); i != end(); ++i) {
      F.push_back(i->F());
    }
  }

  void simple_body::advance(const scalar_time& t) {
    for (iterator i = begin(); i != end(); ++i) {
      i->s(i->s() + i->v()*t + i->a()*t*t/2);
      i->v(i->v() + i->a()*t);
    }
  }

  void simple_body::retreat() {
    for (iterator i = begin(), j = m_backup->begin();
         i != end() && j != m_backup->end();
         ++i, ++j) {
      i->s(j->s());
      i->p(j->p());
    }
  }
}
