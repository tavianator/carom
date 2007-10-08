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

#ifndef CAROM_POLYMORPHIC_LIST_ITERATOR_HPP
#define CAROM_POLYMORPHIC_LIST_ITERATOR_HPP

namespace carom
{
  template<typename T>
  struct master_polymorphic_iterator
  {
  public:
    T* data;
    master_polymorphic_iterator* prior;
    master_polymorphic_iterator* next;
  };

  template<typename T> class polymorphic_const_iterator;

  template<typename T>
  class polymorphic_iterator
  {
    friend class polymorphic_list<T>;
    friend class polymorphic_const_iterator<T>;

  public:
    polymorphic_iterator(const master_polymorphic_iterator<T>* i) : m_i(i) { }
    // polymorphic_iterator(const polymorphic_iterator& i);
    // ~polymorphic_iterator();

    // polymorphic_iterator& operator=(const polymorphic_iterator i);

    T& operator*() const { return *m_i->data; }
    T* operator->() const { return m_i->data; }

    polymorphic_iterator& operator++() { m_i = m_i->next; return *this; }
    polymorphic_iterator& operator--() { m_i = m_i->prior; return *this; }
    const polymorphic_iterator operator++(int)
    { m_i = m_i->next; return polymorphic_iterator(m_i->prior); }
    const polymorphic_iterator operator--(int)
    { m_i = m_i->prior; return polymorphic_iterator(m_i->next); }

  private:
    const master_polymorphic_iterator<T>* m_i;
  };

  template<typename T>
  class polymorphic_const_iterator
  {
    friend class polymorphic_list<T>;

    friend bool operator==(const polymorphic_const_iterator<T>& lhs,
                           const polymorphic_const_iterator<T>& rhs)
    { return lhs.m_i == rhs.m_i; }
    friend bool operator!=(const polymorphic_const_iterator<T>& lhs,
                           const polymorphic_const_iterator<T>& rhs)
    { return lhs.m_i != rhs.m_i; }

  public:
    polymorphic_const_iterator(const master_polymorphic_iterator<T>* i)
      : m_i(i) { }
    polymorphic_const_iterator(const polymorphic_iterator<T>& i)
      : m_i(i.m_i) { }
    // polymorphic_const_iterator(const polymorphic_iterator& i);
    // ~polymorphic_const_iterator();

    // polymorphic_iterator& operator=(const polymorphic_iterator i);

    const T& operator*() const { return *m_i->data; }
    const T* operator->() const { return m_i->data; }

    polymorphic_const_iterator& operator++() { m_i = m_i->next; return *this; }
    polymorphic_const_iterator& operator--() { m_i = m_i->prior; return *this; }
    const polymorphic_const_iterator operator++(int)
    { m_i = m_i->next; return polymorphic_const_iterator(m_i->prior); }
    const polymorphic_const_iterator operator--(int)
    { m_i = m_i->prior; return polymorphic_const_iterator(m_i->next); }

  private:
    const master_polymorphic_iterator<T>* m_i;
  };
}

#endif // CAROM_POLYMORPHIC_LIST_ITERATOR_HPP
