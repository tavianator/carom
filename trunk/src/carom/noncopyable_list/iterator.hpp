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

#ifndef CAROM_NONCOPYABLE_LIST_ITERATOR_HPP
#define CAROM_NONCOPYABLE_LIST_ITERATOR_HPP

namespace carom
{
  template<typename T>
  struct master_noncopyable_iterator
  {
  public:
    T* data;
    master_noncopyable_iterator* prior;
    master_noncopyable_iterator* next;
  };

  template<typename T> class noncopyable_const_iterator;

  template<typename T>
  class noncopyable_iterator
  {
    friend class noncopyable_list<T>;
    friend class noncopyable_const_iterator<T>;

  public:
    noncopyable_iterator(const master_noncopyable_iterator<T>* i) : m_i(i) { }
    // noncopyable_iterator(const noncopyable_iterator& i);
    // ~noncopyable_iterator();

    // noncopyable_iterator& operator=(const noncopyable_iterator i);

    T& operator*() const { return *m_i->data; }
    T* operator->() const { return m_i->data; }

    noncopyable_iterator& operator++() { m_i = m_i->next; return *this; }
    noncopyable_iterator& operator--() { m_i = m_i->prior; return *this; }
    const noncopyable_iterator operator++(int)
    { m_i = m_i->next; return noncopyable_iterator(m_i->prior); }
    const noncopyable_iterator operator--(int)
    { m_i = m_i->prior; return noncopyable_iterator(m_i->next); }

  private:
    const master_noncopyable_iterator<T>* m_i;
  };

  template<typename T>
  class noncopyable_const_iterator
  {
    friend class noncopyable_list<T>;

    friend bool operator==(const noncopyable_const_iterator<T>& lhs,
                           const noncopyable_const_iterator<T>& rhs)
    { return lhs.m_i == rhs.m_i; }
    friend bool operator!=(const noncopyable_const_iterator<T>& lhs,
                           const noncopyable_const_iterator<T>& rhs)
    { return lhs.m_i != rhs.m_i; }

  public:
    noncopyable_const_iterator(const master_noncopyable_iterator<T>* i)
      : m_i(i) { }
    noncopyable_const_iterator(const noncopyable_iterator<T>& i)
      : m_i(i.m_i) { }
    // noncopyable_const_iterator(const noncopyable_iterator& i);
    // ~noncopyable_const_iterator();

    // noncopyable_iterator& operator=(const noncopyable_iterator i);

    const T& operator*() const { return *m_i->data; }
    const T* operator->() const { return m_i->data; }

    noncopyable_const_iterator& operator++() { m_i = m_i->next; return *this; }
    noncopyable_const_iterator& operator--() { m_i = m_i->prior; return *this; }
    const noncopyable_const_iterator operator++(int)
    { m_i = m_i->next; return noncopyable_const_iterator(m_i->prior); }
    const noncopyable_const_iterator operator--(int)
    { m_i = m_i->prior; return noncopyable_const_iterator(m_i->next); }

  private:
    const master_noncopyable_iterator<T>* m_i;
  };
}

#endif // CAROM_NONCOPYABLE_LIST_ITERATOR_HPP
