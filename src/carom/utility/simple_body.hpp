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

#ifndef CAROM_UTILITY_SIMPLE_BODY_HPP
#define CAROM_UTILITY_SIMPLE_BODY_HPP

#include <list>

namespace carom
{
  class simple_body : public body
  {
  public:
    // simple_body();
    // virtual ~simple_body();

    virtual void calculate_k1();
    virtual void calculate_k2();
    virtual void calculate_k3();
    virtual void calculate_k4();

    virtual void apply_k1(const scalar_time& t);
    virtual void apply_k2(const scalar_time& t);
    virtual void apply_k3(const scalar_time& t);

    virtual void apply(const scalar_time& t);

  private:
    body* m_backup;

    std::list<vector_force> m_F1;
    std::list<vector_force> m_F2;
    std::list<vector_force> m_F3;
    std::list<vector_force> m_F4;

    void calculate(std::list<vector_force>& F);
    void advance(const scalar_time& t);
    void retreat();
  };
}

#endif // CAROM_UTILITY_SIMPLE_BODY_HPP
