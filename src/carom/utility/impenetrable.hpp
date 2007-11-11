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

#ifndef CAROM_UTILITY_IMPENETRABLE_HPP
#define CAROM_UTILITY_IMPENETRABLE_HPP

namespace carom
{
  // Abstract base class. A body is considered impenetrable if dynamic_cast'ing
  // it to impenetrable* returns non-null.
  class impenetrable
  {
  public:
    impenetrable() { }
    virtual ~impenetrable() { }

    mesh& surface() { return m_surface; }

    virtual void collision(body& b) = 0;

  private:
    mesh m_surface;

    impenetrable(const impenetrable&);
    impenetrable& operator=(const impenetrable&);
  };

  // An impenetrable body; T should derrive from body
  template <typename T>
  class impenetrable_body : public T, public impenetrable
  {
  public:
    // impenetrable_body();
    // virtual ~impenetrable_body();

    virtual void collision(body& b);
  };
}

#endif // CAROM_UTILITY_IMPENETRABLE_HPP
