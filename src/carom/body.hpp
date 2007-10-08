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

#ifndef CAROM_BODY_HPP
#define CAROM_BODY_HPP

namespace carom
{
  class body
  {
  public:
    // body();
    virtual ~body() { }

    typedef noncopyable_list<particle>::iterator iterator;
    typedef noncopyable_list<particle>::const_iterator const_iterator;

    iterator insert();
    void erase(iterator i);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

  private:
    noncopyable_list<particle> m_particles;

    body(const body&);
    body& operator=(const body&);
  };
}

#endif CAROM_BODY_HPP
