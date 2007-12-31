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

#ifndef CAROM_UTILITY_IMPENETRABLE_HPP
#define CAROM_UTILITY_IMPENETRABLE_HPP

#include <boost/utility.hpp>

namespace carom
{
  // Abstract base class. A body is considered impenetrable if dynamic_cast'ing
  // it to impenetrable* returns non-null.
  class impenetrable : private boost::noncopyable
  {
  public:
    // impenetrable();
    virtual ~impenetrable() { }

    mesh&       surface()       { return m_surface; }
    const mesh& surface() const { return m_surface; }

    virtual void collision(mesh::iterator i, const vector_momentum& I) = 0;

  private:
    mesh m_surface;
  };

  // An impenetrable body; T should derrive from body
  template <typename T>
  class impenetrable_body : public T, public impenetrable
  {
  public:
    // impenetrable_body();
    // virtual ~impenetrable_body();

    virtual void collision(mesh::iterator i, const vector_momentum& I);
  };

  void collision(body& b1, body& b2);

  template <typename T>
  void impenetrable_body<T>::collision(mesh::iterator i,
                                       const vector_momentum& I) {
    i->a->p(i->a->p() + I/3);
    i->b->p(i->b->p() + I/3);
    i->c->p(i->c->p() + I/3);
  }
}

#endif // CAROM_UTILITY_IMPENETRABLE_HPP
