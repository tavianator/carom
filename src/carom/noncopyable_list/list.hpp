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

#ifndef CAROM_NONCOPYABLE_LIST_LIST_HPP
#define CAROM_NONCOPYABLE_LIST_LIST_HPP

#include <algorithm>
#include <iterator>

namespace carom
{
  template<typename T>
  class noncopyable_list
  {
  public:
    typedef T                                     value_type;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef noncopyable_iterator<T>               iterator;
    typedef noncopyable_const_iterator<T>         const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::size_t                           size_type;
    typedef std::ptrdiff_t                        difference_type;

    noncopyable_list();
    ~noncopyable_list() { clear(); }

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
    noncopyable_node<T> m_list;
    noncopyable_node<T> m_end;

    noncopyable_list(const noncopyable_list&);
    noncopyable_list& operator=(const noncopyable_list&);
  };

  template<typename T>
  noncopyable_list<T>::noncopyable_list() {
    m_list.data = 0;
    m_list.prior = 0;
    m_list.next = &m_end;

    m_end.data = 0;
    m_end.prior = &m_list;
    m_end.next = 0;
  }

  template<typename T>
  typename noncopyable_list<T>::iterator
  noncopyable_list<T>::insert(noncopyable_list<T>::iterator pos,
                              noncopyable_list<T>::pointer x) {
    // Transforms this:
    //        ------- ------
    //   ... | prior | next | ...
    //        ------- ------
    // into this:
    //        ------- --- ------
    //   ... | prior | i | next | ...
    //        ------- --- ------

    noncopyable_node<T>* prior = pos.m_i->prior;
    noncopyable_node<T>* next = pos.m_i;
    noncopyable_node<T>* i = new noncopyable_node<T>;

    prior->next = i;

    i->data = x;
    i->prior = prior;
    i->next = next;

    next->prior = i;

    return iterator(i);
  }

  template<typename T>
  void noncopyable_list<T>::erase(noncopyable_list<T>::iterator pos) {
    // Transforms this:
    //        ------- --- ------
    //   ... | prior | i | next | ...
    //        ------- --- ------
    // into this:
    //        ------- ------
    //   ... | prior | next | ...
    //        ------- ------

    noncopyable_node<T>* prior = pos.m_i->prior;
    noncopyable_node<T>* next = pos.m_i->next;
    noncopyable_node<T>* i = pos.m_i;

    delete i->data;
    delete i;

    prior->next = next;
    next->prior = prior;
  }
}

#endif // CAROM_NONCOPYABLE_LIST_LIST_HPP
