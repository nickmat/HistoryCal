/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caljdn.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Jdn (Julian Day Number) calendar header.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     30th December 2013
 * Copyright:   Copyright (c) 2013-2015, Nick Matthews.
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

#include "caljdn.h"

using namespace Cal;
using std::string;


bool Jdn::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    fields[0] = mask[0];
    return fields[0] != f_invalid;
}

bool Jdn::set_fields_as_next_first( Field* fields, const Field* mask ) const
{
    return false;
}

bool Jdn::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    fields[0] = mask[0];
    return fields[0] != f_invalid;
}

bool Jdn::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    return false;
}

int Jdn::get_std_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "day" ) {
        return 0;
    }
    return -1;
}

string Jdn::get_std_fieldname( size_t index ) const
{
    if( index == 0 ) {
        return "day";
    }
    return "";
}

// End of src/cal/caljdn.cpp file
