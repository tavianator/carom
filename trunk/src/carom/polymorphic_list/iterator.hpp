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

#include <boost/utility.hpp>
#include <typeinfo>

namespace carom
{
  template <typename T>
  struct polymorphic_node
  {
  public:
    T* data;
    polymorphic_node* prior;
    polymorphic_node* next;
  };

  // Foreward declarations

  template <typename T>
  class polymorphic_iterator;

  template <typename T>
  class polymorphic_const_iterator;

  // A non-constant iterator for polymorphic_list

  template <typename T>
  class polymorphic_iterator
  {
    friend class polymorphic_list<T>;
    friend class polymorphic_const_iterator<T>;

  public:
    polymorphic_iterator() : m_node(0) { }
    explicit polymorphic_iterator(polymorphic_node<T>* node) : m_node(node) { }
    // polymorphic_iterator(const polymorphic_iterator& i);
    // ~polymorphic_iterator();

    // polymorphic_iterator& operator=(const polymorphic_iterator& i);

    T& operator*() const { return *m_node->data; }
    T* operator->() const { return m_node->data; }

    polymorphic_iterator& operator++() { m_node = m_node->next; return *this; }
    polymorphic_iterator& operator--() { m_node = m_node->prior; return *this; }
    const polymorphic_iterator operator++(int)
    { m_node = m_node->next; return polymorphic_iterator(m_node->prior); }
    const polymorphic_iterator operator--(int)
    { m_node = m_node->prior; return polymorphic_iterator(m_node->next); }

    bool operator==(const polymorphic_const_iterator<T>& i) const
    { return m_node == i.m_node; }
    bool operator!=(const polymorphic_const_iterator<T>& i) const
    { return m_node != i.m_node; }

  private:
    polymorphic_node<T>* m_node;
  };

  // A constant iterator for polymorphic_list

  template <typename T>
  class polymorphic_const_iterator
  {
    friend class polymorphic_list<T>;
    friend class polymorphic_iterator<T>;

  public:
    polymorphic_const_iterator() : m_node(0) { }
    explicit polymorphic_const_iterator(polymorphic_node<T>* node)
      : m_node(node) { }
    polymorphic_const_iterator(const polymorphic_iterator<T>& i)
      : m_node(i.m_node) { }
    // polymorphic_const_iterator(const polymorphic_const_iterator& i);
    // ~polymorphic_const_iterator();

    // polymorphic_const_iterator&
    // operator=(const polymorphic_const_iterator& i);

    const T& operator*() const { return *m_node->data; }
    const T* operator->() const { return m_node->data; }

    polymorphic_const_iterator& operator++()
    { m_node = m_node->next; return *this; }
    polymorphic_const_iterator& operator--()
    { m_node = m_node->prior; return *this; }
    const polymorphic_const_iterator operator++(int)
    { m_node = m_node->next; return polymorphic_const_iterator(m_node->prior); }
    const polymorphic_const_iterator operator--(int)
    { m_node = m_node->prior; return polymorphic_const_iterator(m_node->next); }

    bool operator==(const polymorphic_const_iterator& i) const
    { return m_node == i.m_node; }
    bool operator!=(const polymorphic_const_iterator& i) const
    { return m_node != i.m_node; }

  private:
    polymorphic_node<T>* m_node;
  };

  // A helper class for iterator_cast, to enable behaviour similar to partial
  // template function specialization

  template <typename T>
  struct polymorphic_caster;

  template <typename T>
  struct polymorphic_caster<T&>
  {
  public:
    template <typename U>
    static T& eval(U& data) { return dynamic_cast<T&>(data); }
  };

  template <typename T>
  struct polymorphic_caster<T*>
  {
  public:
    template <typename U>
    static T* eval(U& data)
    { return dynamic_cast<T*>(boost::addressof(data)); }
  };

  // Similar to polymorphic_caster, but requires const in the correct places

  template <typename T>
  struct polymorphic_const_caster;

  template <typename T>
  struct polymorphic_const_caster<const T&>
  {
  public:
    template <typename U>
    static const T& eval(const U& data) { return dynamic_cast<const T&>(data); }
  };

  template <typename T>
  struct polymorphic_const_caster<const T*>
  {
  public:
    template <typename U>
    static const T* eval(const U& data)
    { return dynamic_cast<const T*>(boost::addressof(data)); }
  };

  template <typename T, typename U>
  inline T iterator_cast(const polymorphic_iterator<U>& i) {
    return polymorphic_caster<T>::eval(*i);
  }

  template <typename T, typename U>
  inline T iterator_cast(const polymorphic_const_iterator<U>& i) {
    return polymorphic_const_caster<T>::eval(*i);
  }
}

#endif // CAROM_POLYMORPHIC_LIST_ITERATOR_HPP
