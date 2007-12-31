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

#ifndef CAROM_THREAD_HPP
#define CAROM_THREAD_HPP

#include <pthread.h>
#include <boost/utility.hpp> // For noncopyable

namespace carom
{
  class mutex : private boost::noncopyable
  {
  public:
    mutex() { pthread_mutex_init(&m_mutex, 0); }
    ~mutex() { pthread_mutex_destroy(&m_mutex); }

    void lock() { pthread_mutex_lock(&m_mutex); }
    void unlock() { pthread_mutex_unlock(&m_mutex); }

  private:
    pthread_mutex_t m_mutex;
  };

  class rwlock : private boost::noncopyable
  {
  public:
    rwlock() { pthread_rwlock_init(&m_lock, 0); }
    ~rwlock() { pthread_rwlock_destroy(&m_lock); }

    void read_lock() { pthread_rwlock_rdlock(&m_lock); }
    void write_lock() { pthread_rwlock_wrlock(&m_lock); }
    void unlock() { pthread_rwlock_unlock(&m_lock); }

  private:
    pthread_rwlock_t m_lock;
  };

  class lock : private boost::noncopyable
  {
  public:
    lock(mutex& m) : m_mutex(&m) { m_mutex->lock(); }
    ~lock() { m_mutex->unlock(); }

  private:
    mutex* m_mutex;
  };

  class read_lock : private boost::noncopyable
  {
  public:
    read_lock(rwlock& l) : m_lock(&l) { m_lock->read_lock(); }
    ~read_lock() { m_lock->unlock(); }

  private:
    rwlock* m_lock;
  };

  class write_lock : private boost::noncopyable
  {
  public:
    write_lock(rwlock& l) : m_lock(&l) { m_lock->write_lock(); }
    ~write_lock() { m_lock->unlock(); }

  private:
    rwlock* m_lock;
  };
}

#endif // CAROM_THREAD_HPP
