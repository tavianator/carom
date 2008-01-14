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

#ifndef CAROM_UTILITY_ELECTROMAGNETISM_HPP
#define CAROM_UTILITY_ELECTROMAGNETISM_HPP

#include <boost/utility.hpp> // For noncopyable

namespace carom
{
  // More useful typedefs
  typedef scalar_units<0, 0, 0>  scalar_charge;
  typedef vector_units<1, 1, -2> vector_electric_field;
  typedef vector_units<1, 0, -1> vector_magnetic_field;

  class charge : private boost::noncopyable
  {
  public:
    scalar_charge q() const { return m_charge; }

    void q(const scalar_charge& q) { m_charge = q; }

  private:
    scalar_charge m_charge;
  };

  template <typename T> // T should derrive from particle
  class charged_particle : public T, public charge
  {
  };

  class electric_force : public applied_force
  {
  public:
    electric_force(const vector_electric_field& E) : m_E(E) { }
  };
}

#endif // CAROM_UTILITY_ELECTROMAGNETISM_HPP
