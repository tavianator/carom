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
  boost::thread_specific_ptr<optimization>* pool_ptr;

  int pool_init::s_refcount;
  optimization* pool_init::s_context;

  pool_init::pool_init() {
    // To ensure that s_refcount is set to zero, we use a local static variable
    static int refcount = 0;

    if (refcount == 0) {
      pool_ptr = new boost::thread_specific_ptr<optimization>();
      s_context = new optimization();
    }

    ++refcount;
    s_refcount = refcount;
  }

  pool_init::~pool_init() {
    if (s_refcount == 0) {
      delete s_context;
      delete pool_ptr;
    }

    --s_refcount;
  }
}
