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
    iterator       end()         { return iterator(m_end); }
    const_iterator end() const   { return const_iterator(m_end); }

    reverse_iterator       rbegin() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const
    { return const_reverse_iterator(begin()); }
    reverse_iterator       rend() { return reverse_iterator(end()); }
    const_reverse_iterator rend() const 
    { return const_reverse_iterator(end()); }

    bool empty() { return m_list.next == m_end; }
    size_type size() const { return std::distance(begin(), end()); }

    reference       front()       { return *m_list.next->data; }
    const_reference front() const { return *m_list.next->data; }
    reference       back()        { return *m_end->prior->data; }
    const_reference back() const  { return *m_end->prior->data; }

    void push_front(pointer x) { insert(begin(), x); }
    void pop_front() { erase(begin()); }
    void push_back(pointer x) { insert(end(), x); }
    void pop_back() { erase(end()); }

    iterator insert(iterator pos, pointer x);
    void erase(iterator pos);

    void clear();

  private:
    master_noncopyable_iterator<T> m_list;
    master_noncopyable_iterator<T>* m_end;

    noncopyable_list(const noncopyable_list&);
    noncopyable_list& operator=(const noncopyable_list&);
  };
}

#endif // CAROM_NONCOPYABLE_LIST_LIST_HPP
