/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calformat.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Format class to control the formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th November 2013
 * Copyright:   Copyright (c) 2013 ~ 2016, Nick Matthews.
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

#include "calformat.h"

#include "calbase.h"
#include "calgrammar.h"
#include "calparse.h"
#include "calrecord.h"
#include "caltext.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;


string Format::rlist_to_string( Base* base, const RangeList& ranges ) const
{
    string str;
    for( size_t i = 0 ; i < ranges.size() ; i++ ) {
        if( i > 0 ) {
            str += " | ";
        }
        str += range_to_string( base, ranges[i] );
    }
    return str;
}

string Format::range_to_string( Base* base, Range range ) const
{
    if( range.jdn1 == range.jdn2 ) {
        return jdn_to_string( base, range.jdn1 );
    }
    string str1, str2;
    Record rec1( base, range.jdn1 );
    Record rec2( base, range.jdn2 );

    rec1.remove_balanced_fields( &rec2 );
    str1 = get_output( rec1 );
    str2 = get_output( rec2 );
    if( str1 == str2 ) {
        return str1;
    }
    return str1 + " ~ " + str2;
}

string Format::jdn_to_string( Base* base, Field jdn ) const
{
    if( jdn == f_minimum ) {
        return "past";
    }
    if( jdn == f_maximum ) {
        return "future";
    }
    Record rec( base, jdn );
    return get_output( rec );
}

// End of src/cal/calformat.cpp file
