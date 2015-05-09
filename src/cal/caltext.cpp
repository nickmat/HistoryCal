/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caltext.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Miscellaneous text functions.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     6th May 2015
 * Copyright:   Copyright (c) 2015, Nick Matthews.
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

#include "caltext.h"

using namespace Cal;
using std::string;

string Cal::get_ordinal_suffix( Field field )
{
    if( (field % 100) < 4 || (field % 100) > 20 ) {
        switch( field % 10 )
        {
        case 1:
            return "st";
        case 2:
            return "nd";
        case 3:
            return "rd";
        }
    }
    return "th";
}

string Cal::get_ordinal_suffix_style()
{
    return "th";
}

// End of src/cal/caltext.cpp
