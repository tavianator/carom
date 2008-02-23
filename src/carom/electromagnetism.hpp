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

#ifndef CAROM_ELECTROMAGNETISM_HPP
#define CAROM_ELECTROMAGNETISM_HPP

#include <boost/utility.hpp> // For noncopyable

namespace carom
{
  // More useful typedefs
  typedef scalar_units<0, 0, 0>   scalar_charge;
  typedef scalar_units<1, 1, -2>  scalar_electric_field;
  typedef scalar_units<1, 0, -1>  scalar_magnetic_field;
  typedef vector_units<1, 1, -2>  vector_electric_field;
  typedef vector_units<1, 0, -1>  vector_magnetic_field;
  typedef scalar_units<-1, -3, 2> scalar_permitivity_constant;
  typedef scalar_units<1, 1, 0>   scalar_permiability_constant;

  class charge : private boost::noncopyable
  {
  public:
    scalar_charge q() const;
    void q(const scalar_charge& q);

  private:
    scalar_charge m_charge;
  };

  template <typename T> // T should derrive from particle
  class charged_particle : public T, public charge
  {
  };

  class electromagnetic_force : public applied_force
  {
  public:
    electromagnetic_force(const particle& x);
    virtual ~electromagnetic_force();

    static scalar_permitivity_constant e();
    static scalar_permiability_constant u();
    static void e(const scalar_permitivity_constant& e);
    static void u(const scalar_permiability_constant& u);

    virtual vector_force force(const particle& x) const;

  private:
    const particle* m_x;
    const charge* m_q;

    static scalar_permitivity_constant s_e;
    static scalar_permiability_constant  s_u;
  };

  class electric_force : public applied_force
  {
  public:
    electric_force(const vector_electric_field& E);
    virtual ~electric_force();

    virtual vector_force force(const particle& x) const;

  private:
    vector_electric_field m_E;
  };

  class magnetic_force : public applied_force
  {
  public:
    magnetic_force(const vector_magnetic_field& B);
    virtual ~magnetic_force();

    virtual vector_force force(const particle& x) const;

  private:
    vector_magnetic_field m_B;
  };
}

#endif // CAROM_ELECTROMAGNETISM_HPP
