/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calpattern.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Class to encapulate a recurring date or range.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     11th June 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  The Cal library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The Cal library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the Cal library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#ifndef CAL_CALPATTERN_H_GUARD
#define CAL_CALPATTERN_H_GUARD

#include "cal/caldefs.h"

namespace Cal {

    class Base;

    class Pattern
    {
    public:
        enum Rule { PR_Match, PR_Before, PR_After };

        Pattern( Base* base );

        void set_mask( const std::string& mask );
        void set_rule( Rule rule ) { m_rule = rule; }

    private:
        Base*     m_base;
        FieldVec  m_mask;
        Rule      m_rule;
    };

}

#endif // CAL_CALPATTERN_H_GUARD
