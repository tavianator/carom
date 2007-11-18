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
#include <string>

namespace carom
{
  inline void precision(unsigned long prec) { mpfr_set_default_prec(prec); }
  inline unsigned long precision() { return mpfr_get_default_prec(); }

  inline void mpfr_from(mpfr_t rop, signed short n)
  { mpfr_set_si(rop, n, GMP_RNDN); }
  inline void mpfr_from(mpfr_t rop, unsigned short n)
  { mpfr_set_ui(rop, n, GMP_RNDN); }
  inline void mpfr_from(mpfr_t rop, signed int n)
  { mpfr_set_si(rop, n, GMP_RNDN); }
  inline void mpfr_from(mpfr_t rop, unsigned int n)
  { mpfr_set_ui(rop, n, GMP_RNDN); }
  inline void mpfr_from(mpfr_t rop, signed long n)
  { mpfr_set_si(rop, n, GMP_RNDN); }
  inline void mpfr_from(mpfr_t rop, unsigned long n)
  { mpfr_set_ui(rop, n, GMP_RNDN); }

  inline void mpfr_from(mpfr_t rop, const char* str)
  { mpfr_set_str(rop, str, 0, GMP_RNDN); }
  inline void mpfr_from(mpfr_t rop, const std::string& str)
  { mpfr_set_str(rop, str.c_str(), 0, GMP_RNDN); }

  template <typename T> T mpfr_to(mpfr_t fp);

  template <>
  inline float mpfr_to<float>(mpfr_t fp)
  { return mpfr_get_d(fp, GMP_RNDN); }

  template <>
  inline double mpfr_to<double>(mpfr_t fp)
  { return mpfr_get_d(fp, GMP_RNDN); }

  template <>
  inline long double mpfr_to<long double>(mpfr_t fp)
  { return mpfr_get_d(fp, GMP_RNDN); }

  template <>
  inline signed short mpfr_to<signed short>(mpfr_t fp)
  { return mpfr_get_si(fp, GMP_RNDN); }

  template <>
  inline unsigned short mpfr_to<unsigned short>(mpfr_t fp)
  { return mpfr_get_ui(fp, GMP_RNDN); }

  template <>
  inline signed int mpfr_to<signed int>(mpfr_t fp)
  { return mpfr_get_si(fp, GMP_RNDN); }

  template <>
  inline unsigned int mpfr_to<unsigned int>(mpfr_t fp)
  { return mpfr_get_ui(fp, GMP_RNDN); }

  template <>
  inline signed long mpfr_to<signed long>(mpfr_t fp)
  { return mpfr_get_si(fp, GMP_RNDN); }

  template <>
  inline unsigned long mpfr_to<unsigned long>(mpfr_t fp)
  { return mpfr_get_ui(fp, GMP_RNDN); }
}

#endif // CAROM_MPFR_UTILS_HPP
