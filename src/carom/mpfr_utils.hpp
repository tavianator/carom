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

#ifndef CAROM_MPFR_UTILS_HPP
#define CAROM_MPFR_UTILS_HPP

#include <mpfr.h>

namespace carom
{
  void precision(unsigned long prec) { mpfr_set_default_prec(prec); }
  unsigned long precision() { return mpfr_get_default_prec(); }

  template <typename T> const T mpfr_to(mpfr_t fp);
  template <typename T> void mpfr_from(mpfr_t rop, const T& n);
}

#endif // CAROM_MPFR_UTILS_HPP
