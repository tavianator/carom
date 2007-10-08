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

#ifndef CAROM_NONCOPYABLE_LIST_HPP
#define CAROM_NONCOPYABLE_LIST_HPP

#include <list>

namespace carom
{
  template<typename T>
  class noncopyable_list
  {
  public:
    // noncopyable_list();
    // ~noncopyable_list();

  private:
    noncopyable_list(const noncopyable_list&);
    noncopyable_list& operator=(const noncopyable_list&);
  };
}

#endif CAROM_NONCOPYABLE_LIST_HPP
