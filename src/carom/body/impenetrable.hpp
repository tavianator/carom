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

#ifndef CAROM_BODY_IMPENETRABLE_HPP
#define CAROM_BODY_IMPENETRABLE_HPP

namespace carom
{
  class impenetrable
  {
  public:
    impenetrable() { }
    virtual ~impenetrable() { }

  private:
    impenetrable(const impenetrable&);
    impenetrable& operator=(const impenetrable&);
  };

  template<typename T>
  class impenetrable_body : public T, public impenetrable
  {
  public:
    // impenetrable_body();
    // virtual ~impenetrable_body();
  };
}

#endif // CAROM_BODY_IMPENETRABLE_HPP
