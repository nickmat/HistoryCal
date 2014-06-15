/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calpattern.cpp
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

#include "calpattern.h"

#include "calrecord.h"
#include "calbase.h"

using namespace Cal;
using namespace std;


Pattern::Pattern( Base* base )
    : m_base(base), m_rule(PR_Match)
{
}

void Pattern::set_mask( const std::string& mask )
{
    Record rec( m_base, mask );
    m_mask = rec.get_fieldvec();
}

// End of src/cal/calpattern.cpp file
