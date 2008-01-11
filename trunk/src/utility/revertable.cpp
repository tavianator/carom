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

#include <carom.hpp>

namespace carom
{
  void revertable_body::begin_integration() {
    m_backup = new body();

    for (const_iterator i = begin(); i != end(); ++i) {
      iterator j = m_backup->insert(new particle());
      j->m(i->m());
      j->s(i->s());
      j->p(i->p());
    }
  }

  void revertable_body::end_integration() {
    clear_forces(*this);
    delete m_backup;
  }

  void revertable_body::revert() {
    for (iterator i = begin(), j = m_backup->begin();
         i != end() && j != m_backup->end();
         ++i, ++j) {
      i->s(j->s());
      i->p(j->p());
    }
  }
}
