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

#ifndef CAROM_POLYMORPHIC_LIST_HPP
#define CAROM_POLYMORPHIC_LIST_HPP

namespace carom
{
  template<typename T>
  class polymorphic_list
  {
  public:
    // polymorphic_list();
    // ~polymorphic_list();

  private:
    polymorphic_list(const polymorphic_list&);
    polymorphic_list& operator=(const polymorphic_list&);
  };
}

#endif CAROM_POLYMORPHIC_LIST_HPP
