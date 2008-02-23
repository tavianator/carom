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

#ifndef CAROM_POLYMORPHIC_LIST_HPP
#define CAROM_POLYMORPHIC_LIST_HPP

#include <boost/utility.hpp> // for addressof, noncopyable
#include <algorithm> // for distance
#include <cstddef> // for ptrdiff_t
#include <iterator> // for biderectional_iterator_tag, reverse_iterator
#include <typeinfo> // for dynamic_cast

namespace carom
{
  // Foreward declaration
  template <typename T> class polymorphic_list;

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
    typedef std::ptrdiff_t                  difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T                               value_type;
    typedef T*                              pointer;
    typedef T&                              reference;

    polymorphic_iterator() : m_node(0) { }
    explicit polymorphic_iterator(polymorphic_node<T>* node) : m_node(node) { }
    // polymorphic_iterator(const polymorphic_iterator& i);
    // ~polymorphic_iterator();

    // polymorphic_iterator& operator=(const polymorphic_iterator& i);

    reference operator*() const { return *m_node->data; }
    pointer operator->() const { return m_node->data; }

    polymorphic_iterator& operator++() { m_node = m_node->next; return *this; }
    polymorphic_iterator& operator--() { m_node = m_node->prior; return *this; }
    polymorphic_iterator operator++(int)
    { m_node = m_node->next; return polymorphic_iterator(m_node->prior); }
    polymorphic_iterator operator--(int)
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
    typedef std::ptrdiff_t                  difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T                               value_type;
    typedef const T*                        pointer;
    typedef const T&                        reference;

    polymorphic_const_iterator() : m_node(0) { }
    explicit polymorphic_const_iterator(polymorphic_node<T>* node)
      : m_node(node) { }
    polymorphic_const_iterator(const polymorphic_iterator<T>& i)
      : m_node(i.m_node) { }
    // polymorphic_const_iterator(const polymorphic_const_iterator& i);
    // ~polymorphic_const_iterator();

    // polymorphic_const_iterator&
    // operator=(const polymorphic_const_iterator& i);

    reference operator*() const { return *m_node->data; }
    pointer operator->() const { return m_node->data; }

    polymorphic_const_iterator& operator++()
    { m_node = m_node->next; return *this; }
    polymorphic_const_iterator& operator--()
    { m_node = m_node->prior; return *this; }
    polymorphic_const_iterator operator++(int)
    { m_node = m_node->next; return polymorphic_const_iterator(m_node->prior); }
    polymorphic_const_iterator operator--(int)
    { m_node = m_node->prior; return polymorphic_const_iterator(m_node->next); }

    bool operator==(const polymorphic_const_iterator& i) const
    { return m_node == i.m_node; }
    bool operator!=(const polymorphic_const_iterator& i) const
    { return m_node != i.m_node; }

  private:
    polymorphic_node<T>* m_node;
  };

  // The polymorphic_list class itself
  template <typename T>
  class polymorphic_list : private boost::noncopyable
  {
  public:
    typedef T                                     value_type;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef polymorphic_iterator<T>               iterator;
    typedef polymorphic_const_iterator<T>         const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::size_t                           size_type;
    typedef std::ptrdiff_t                        difference_type;

    polymorphic_list();
    ~polymorphic_list() { clear(); }

    iterator       begin()       { return iterator(m_list.next); }
    const_iterator begin() const { return const_iterator(m_list.next); }
    iterator       end()         { return iterator(&m_end); }
    const_iterator end() const   { return const_iterator(&m_end); }

    reverse_iterator       rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const
    { return const_reverse_iterator(end()); }
    reverse_iterator       rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const 
    { return const_reverse_iterator(begin()); }

    bool empty() { return m_list.next == &m_end; }
    size_type size() const { return std::distance(begin(), end()); }

    reference       front()       { return *m_list.next->data; }
    const_reference front() const { return *m_list.next->data; }
    reference       back()        { return *m_end.prior->data; }
    const_reference back() const  { return *m_end.prior->data; }

    void push_front(pointer x) { insert(begin(), x); }
    void pop_front() { erase(begin()); }
    void push_back(pointer x) { insert(end(), x); }
    void pop_back() { erase(end()); }

    iterator insert(iterator pos, pointer x);
    void erase(iterator pos);

    void clear() { while (!empty()) { erase(begin()); } }

  private:
    mutable polymorphic_node<T> m_list;
    mutable polymorphic_node<T> m_end;
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

  // To dynamic_cast from an iterator to a derived type pointer or reference

  template <typename T, typename U>
  inline T iterator_cast(const polymorphic_iterator<U>& i) {
    return polymorphic_caster<T>::eval(*i);
  }

  template <typename T, typename U>
  inline T iterator_cast(const polymorphic_const_iterator<U>& i) {
    return polymorphic_const_caster<T>::eval(*i);
  }

  // polymorphic_list implementation

  template <typename T>
  polymorphic_list<T>::polymorphic_list() {
    m_list.data = 0;
    m_list.prior = 0;
    m_list.next = &m_end;

    m_end.data = 0;
    m_end.prior = &m_list;
    m_end.next = 0;
  }

  template <typename T>
  typename polymorphic_list<T>::iterator
  polymorphic_list<T>::insert(polymorphic_list<T>::iterator pos,
                              polymorphic_list<T>::pointer x) {
    // Transforms this:
    //        ------- ------
    //   ... | prior | next | ...
    //        ------- ------
    // into this:
    //        ------- --- ------
    //   ... | prior | i | next | ...
    //        ------- --- ------

    polymorphic_node<T>* prior = pos.m_node->prior;
    polymorphic_node<T>* next = pos.m_node;
    polymorphic_node<T>* i = new polymorphic_node<T>;

    prior->next = i;

    i->data = x;
    i->prior = prior;
    i->next = next;

    next->prior = i;

    return iterator(i);
  }

  template <typename T>
  void polymorphic_list<T>::erase(polymorphic_list<T>::iterator pos) {
    // Transforms this:
    //        ------- --- ------
    //   ... | prior | i | next | ...
    //        ------- --- ------
    // into this:
    //        ------- ------
    //   ... | prior | next | ...
    //        ------- ------

    polymorphic_node<T>* prior = pos.m_node->prior;
    polymorphic_node<T>* next = pos.m_node->next;
    polymorphic_node<T>* i = pos.m_node;

    delete i->data;
    delete i;

    prior->next = next;
    next->prior = prior;
  }
}

#include <carom/polymorphic_list/iterator.hpp>
#include <carom/polymorphic_list/list.hpp>

#endif // CAROM_POLYMORPHIC_LIST_HPP
