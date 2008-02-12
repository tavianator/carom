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

#ifndef CAROM_UTILITY_BASIC_FORCES_HPP
#define CAROM_UTILITY_BASIC_FORCES_HPP

namespace carom
{
  // More useful typedefs
  typedef scalar_units<-1, 3, -1> scalar_gravitational_constant;
  typedef scalar_units<1, 0, -2> scalar_spring_constant;

  class constant_force : public applied_force
  {
  public:
    constant_force(const vector_force& F);
    virtual ~constant_force();

    virtual vector_force force(const particle& x) const;

  private:
    vector_force m_F;
  };

  class centripetal_force : public applied_force
  {
  public:
    centripetal_force(const vector_displacement& o);
    virtual ~centripetal_force();

    virtual vector_force force(const particle& x) const;

  private:
    vector_displacement m_o;
  };

  class gravitational_force : public applied_force
  {
  public:
    gravitational_force(const particle& x);
    virtual ~gravitational_force();

    static scalar_gravitational_constant G();
    static void G(const scalar_gravitational_constant& G);

    virtual vector_force force(const particle& x) const;

  private:
    const particle* m_x;

    static scalar_gravitational_constant s_G;
  };

  class spring_force : public applied_force
  {
  public:
    spring_force(const vector_displacement& o, const scalar_distance& l,
                 const scalar_spring_constant& k);
    virtual ~spring_force();

    virtual vector_force force(const particle& x) const;

  private:
    vector_displacement m_o;
    scalar_distance m_l;
    scalar_spring_constant m_k;
  };
}

#endif // CAROM_UTILITY_BASIC_FORCES_HPP
