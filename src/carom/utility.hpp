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

#ifndef CAROM_UTILITY_HPP
#define CAROM_UTILITY_HPP

namespace carom
{
  scalar_mass         mass          (const body& b);
  vector_displacement center_of_mass(const body& b);
  vector_velocity     velocity      (const body& b);
  vector_momentum     momentum      (const body& b);
  vector_acceleration acceleration  (const body& b);
  vector_force        force         (const body& b);

  scalar_moment_of_inertia
  moment_of_inertia(const body& b, const vector_displacement& o,
                    const vector& axis);

  vector_angular_velocity
  angular_velocity(const body& b, const vector_displacement& o,
                   const vector& axis);

  vector_angular_momentum
  angular_momentum(const body& b, const vector_displacement& o);

  vector_angular_acceleration
  angular_acceleration(const body& b, const vector_displacement& o,
                       const vector& axis);

  vector_torque torque(const body& b, const vector_displacement& o);

  void apply_forces(body& b);
  void clear_forces(body& b);
}

#include <carom/utility/mesh.hpp>
#include <carom/utility/impenetrable.hpp>

#include <carom/utility/simple_body.hpp>
#include <carom/utility/rigid_body.hpp>
#include <carom/utility/fluid_body.hpp>

#endif // CAROM_UTILITY_HPP
