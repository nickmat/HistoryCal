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

#include <sstream>

using namespace Cal;
using std::string;

string Cal::field_to_str( Field field )
{
    if( field== f_invalid ) {
        return "";
    }
    if( field == f_maximum ) {
        return "future";
    }
    if( field == f_minimum ) {
        return "past";
    }
    std::ostringstream ss; 
    ss << field; 
    return ss.str();
}

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

string Cal::get_roman_numerals( Field field )
{
    // We can only convert numbers 1 to 4999 (4000 = "MMMM")
    if( field >= 5000 || field <= 0 ) {
        return field_to_str( field );
    }
    size_t n = field;
    static const char* units[10]
        = { "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };
    static const char* tens[10]
        = { "", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC" };
    static const char* hunds[10]
        = { "", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM" };
    static const char* thous[5]
        = { "", "M", "MM", "MMM", "MMMM" };

    return string(thous[(n/1000)%10]) 
        + string(hunds[(n/100)%10]) 
        + string(tens[(n/10)%10]) 
        + string(units[n%10]);
}

// End of src/cal/caltext.cpp
