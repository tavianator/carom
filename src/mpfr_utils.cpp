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
#include <boost/thread.hpp>

namespace carom
{
  boost::thread_specific_ptr<optimization> pool;

  optimization::optimization() {
    m_backup = pool.release();
    pool.reset(this);
  }

  optimization::~optimization() {
    while (!m_list.empty()) {
      mpfr_clear(m_list.front());
      delete m_list.front();
      m_list.pop_front();
    }

    pool.release();
    pool.reset(m_backup);
  }

  mpfr_ptr optimization::acquire() {
    mpfr_ptr r;

    if (m_list.empty()) {
      r = new __mpfr_struct;
      mpfr_init2(r, precision());
    } else {
      r = m_list.front();
      m_list.pop_front();
      mpfr_set_prec(r, precision()); // Usually a no-op
    }

    return r;
  }

  void optimization::release(mpfr_ptr op) {
    m_list.push_back(op);
  }
}
