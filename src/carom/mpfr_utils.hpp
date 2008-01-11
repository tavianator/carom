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
#include <boost/utility.hpp> // For noncopyable
#include <boost/thread.hpp> // For thread_specific_ptr
#include <string>
#include <list>
#include <iostream>

namespace carom
{
  inline void precision(unsigned long prec) { mpfr_set_default_prec(prec); }
  inline unsigned long precision() { return mpfr_get_default_prec(); }

  class optimization;

  extern boost::thread_specific_ptr<optimization>* pool_ptr;

  // A simple, useful optimization. Rather than call mpfr_init for every scalar
  // or vector constructed, this class stores mpfr_ptr's in a list, and returns
  // already initialized ones when it can.
  class optimization : private boost::noncopyable
  {
  public:
    optimization() {
      m_backup = pool_ptr->release();
      pool_ptr->reset(this);
    }

    ~optimization() {
      while (!m_list.empty()) {
        mpfr_clear(m_list.front());
        delete m_list.front();
        m_list.pop_front();
      }

      pool_ptr->release();
      pool_ptr->reset(m_backup);
    }

    mpfr_ptr acquire() {
      mpfr_ptr r;

      if (m_list.empty()) {
        r = new __mpfr_struct;
        mpfr_init2(r, precision());
      } else {
        r = m_list.front();
        m_list.pop_front();
        mpfr_set_prec(r, precision()); // Usually a no-op
      }

      return r;
    }

    void release(mpfr_ptr op) { m_list.push_back(op); }

  private:
    std::list<mpfr_ptr> m_list;
    optimization* m_backup;
  };

  // Returns a thread-specific instance of the optimization class
  inline optimization& pool() { return **pool_ptr; }

  // A helper class, working much like std::ios_base::Init, to ensure that
  // pool() always returns a constructed optimization
  class pool_init
  {
  public:
    pool_init();
    ~pool_init();

  private:
    static int s_refcount;
    static optimization* s_context;
  };

  // Every translation unit has an instance of this class constructed before
  // anything can possibly use pool()
  namespace
  {
    pool_init piniter_;
  }

  // Helper routines for working with the MPFR library

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
