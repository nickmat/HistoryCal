/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/caltext.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Miscellaneous text functions.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     6th May 2015
 * Copyright:   Copyright (c) 2015 ~ 2017, Nick Matthews.
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

#include <cassert>
#include <cstdlib>
#include <sstream>

using namespace Cal;
using std::string;


static string ascii_tolower( const string& str )
{
    string result;
    for( string::const_iterator it = str.begin() ; it != str.end() ; it++ ) {
        if( *it >= 'A' && *it <= 'Z' ) {
            result += *it + ('a' - 'A');
        } else {
            result += *it;
        }
    }
    return result;
}

static void ascii_tolower( string& str )
{
    for( string::iterator it = str.begin() ; it != str.end() ; it++ ) {
        if( *it >= 'A' && *it <= 'Z' ) {
            *it = *it + ('a' - 'A');
        }
    }
}

string Cal::field_to_str( Field field, ShowInvalid si )
{
    if( field== f_invalid ) {
        if( si == SI_qmark ) {
            return "?";
        }
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

string Cal::dual_fields_to_str( Field field, Field dual )
{
    string result = field_to_str( field );
    string dualstr = field_to_str( dual );
    if( result.size() && dualstr.size() && dualstr != result ) {
        if( result.size() != dualstr.size() ) {
            return result + "/" + dualstr;
        }
        string suffix = "/";
        bool matched = true;
        for( string::iterator rit = result.begin(), dit = dualstr.begin()
            ; rit != result.end() ; rit++, dit++
        ) {
            if( matched && *rit != *dit ) {
                matched = false;
            }
            if( ! matched ) {
                suffix += *dit;
            }
        }
        result += suffix;
    }
    return result;
}


string Cal::get_ordinal_suffix( Field field, StringStyle style )
{
    if( field <= f_minimum || field >= f_maximum ) {
        return "";
    }
    const char* result = "TH";
    if( (field % 100) < 4 || (field % 100) > 20 ) {
        switch( field % 10 )
        {
        case 1:
            result = "ST";
            break;
        case 2:
            result = "ND";
            break;
        case 3:
            result = "RD";
            break;
        }
    }
    string str(result);
    if( style != SS_uppercase ) {
        ascii_tolower( str );
    }
    return str;
}

string Cal::get_ordinal_suffix_style( StringStyle style )
{
    return style == SS_uppercase ? "TH" : "th";
}

string Cal::get_roman_numerals( Field field, StringStyle style )
{
    // We can only convert numbers 1 to 4999 (4000 = "MMMM")
    if( field >= 5000 || field <= 0 ) {
        return field_to_str( field );
    }
    static const char* units[10]
        = { "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };
    static const char* tens[10]
        = { "", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC" };
    static const char* hunds[10]
        = { "", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM" };
    static const char* thous[5]
        = { "", "M", "MM", "MMM", "MMMM" };

    size_t n = field;
    string result = string(thous[n/1000])
        + string(hunds[(n/100)%10])
        + string(tens[(n/10)%10])
        + string(units[n%10]);

    if( style == SS_lowercase ) {
        ascii_tolower( result );
    }
    return result;
}

string Cal::get_roman_numerals_style( StringStyle style )
{
    return style == SS_lowercase ? "[x]" : "[X]";
}


static string left_padded_str( const string& str, const string& ch, size_t size )
{
    assert( ch.size() > 0 );
    string result = str;
    while( result.size() < size ) {
        result = ch + result;
    }
    return result;
}

string Cal::get_left_padded( Field field, const std::string& specifier )
{
    // TODO: This assumes fieldstyle and specifier are ASCII.
    // Rewrite for utf8.
    if( specifier.size() < 2 ) {
        return field_to_str( field );
    }
    size_t width = std::strtol( specifier.substr( 1 ).c_str(), NULL, 10 );
    string ch = specifier.substr( 0, 1 );
    return get_left_padded( field, ch, width ); 
}

string Cal::get_left_padded( Field field, const std::string& ch, size_t width )
{
    if( field == f_invalid ) {
        return "";
    }
    if( ch.empty() ) {
        return field_to_str( field );
    }
    bool neg = ( field < 0 && ch == "0" );
    if( neg ) {
        field = -field;
    }
    string result = field_to_str( field );
    if( result.size() < width ) {
        result = left_padded_str( result, ch, width );
    }
    if( neg ) {
        result = "-" + result;
    }
    return result;
}

string Cal::get_left_pad_style(
        const string& fieldstyle, const string& specifier )
{
    // TODO: This assumes fieldstyle and specifier are ASCII.
    // Rewrite for utf8.
    if( specifier.size() < 2 ) {
        return fieldstyle;
    }
    size_t width = std::strtol( specifier.substr( 1 ).c_str(), NULL, 10 );
    if( width < 2 ) {
        return fieldstyle;
    }
    string ch = specifier.substr( 0, 1 );
    string style = fieldstyle;
    if ( style.size() >= width ) {
        style = fieldstyle.substr( fieldstyle.size() - ( width - 1 ), width - 1 );
    }
    return left_padded_str( style, ch, width );
}


// End of src/cal/caltext.cpp
