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

#ifndef CAROM_SYSTEM_HPP
#define CAROM_SYSTEM_HPP

namespace carom
{
  class system
  {
  public:
    typedef polymorphic_list<body>::iterator iterator;
    typedef polymorphic_list<body>::const_iterator const_iterator;

    // system();
    // ~system();

    iterator insert();
    void erase(iterator i);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    void integrate(const scalar_time& t);

  private:
    polymorphic_list<body> m_bodies;

    system(const system&);
    system& operator=(const system&);
  };
}

#endif // CAROM_SYSTEM_HPP
