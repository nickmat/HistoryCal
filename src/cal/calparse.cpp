/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calparse.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Functions to help parse scripts.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
 * Copyright:   Copyright (c) 2013 ~ 2020, Nick Matthews.
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

#include "calparse.h"

#include "caltext.h"

#include <utf8/utf8api.h>

#include <cassert>
#include <cstdlib>

using namespace Cal;
using std::string;


string Cal::left_trim( const string& str )
{
    size_t pos = str.find_first_not_of( " " );
    return ( pos == string::npos ) ? "" : str.substr( pos );
}

std::string Cal::right_trim( const string& str )
{
    size_t pos = str.find_last_not_of( " " );
    return ( pos == string::npos ) ? "" : str.substr( 0, pos + 1 );
}

string Cal::full_trim( const string& str )
{
    return right_trim( left_trim( str ) );
}

string Cal::get_first_word( const string& str, string* tail, char sep )
{
    size_t pos = str.find( sep );
    // Note, tail may point str (ie, str and tail could be the same),
    // so get result before setting tail.
    string result = str.substr( 0, pos );
    if( tail ) {
        *tail = ( pos == string::npos ) ? "" : left_trim( str.substr( pos + 1 ) );
    }
    return result;
}

Field Cal::str_to_field( const std::string& str )
{
    return std::strtol( str.c_str(), NULL, 10 );
}

Field Cal::str_to_dual2( Field dual1, const string& str2 )
{
    string str1 = field_to_str( dual1 );
    if( str1.size() <= str2.size() ) {
        return str_to_field( str2 );
    }
    string result;
    string::const_iterator it1 = str1.end(), it2 = str2.end();
    while( it1 != str1.begin() ) {
        --it1;
        if( it2 != str2.begin() ) {
            --it2;
            result = *it2 + result;
        } else {
            result = *it1 + result;
        }
    }
    return str_to_field( result );
}

double Cal::str_to_double( const std::string& str )
{
    return std::strtod( str.c_str(), NULL );
}

static Field get_roman_value( int ch, bool& decimal )
{
    switch ( ch )
    {
    case 'I': case 'i': decimal = true;  return 1;
    case 'V': case 'v': decimal = false; return 5;
    case 'X': case 'x': decimal = true;  return 10;
    case 'L': case 'l': decimal = false; return 50;
    case 'C': case 'c': decimal = true;  return 100;
    case 'D': case 'd': decimal = false; return 500;
    case 'M': case 'm': decimal = true;  return 1000;
    default:  decimal = false; return f_invalid;
    }
}

Field Cal::convert_roman_numerals( const string& num )
{
    Field result = 0, prev = 1001, curr;
    bool dec = false;
    for ( auto it = num.begin(); it != num.end(); it++ ) {
        curr = get_roman_value( *it, dec );
        if ( curr == f_invalid ) {
            return f_invalid;
        }
        if ( curr >= prev ) {
            return f_invalid;
        }
        prev = curr;
        auto it2 = it + 1;
        if ( it2 != num.end() ) {
            bool dec2;
            Field curr2 = get_roman_value( *it2, dec2 );
            if ( curr2 == f_invalid ) {
                return f_invalid;
            }
            if ( dec ) {
                if ( curr == curr2 ) { // Repeated char
                    auto it3 = it2 + 1;
                    if ( it3 != num.end() ) {
                        Field curr3 = get_roman_value( *it3, dec2 );
                        if ( curr3 == curr ) {
                            result += curr;
                            it++;
                        }
                    }
                    result += curr;
                    it++;
                } else if ( ( curr == 1 && ( curr2 == 5 || curr2 == 10 ) ) ||
                    ( curr == 10 && ( curr2 == 50 || curr2 == 100 ) ) ||
                    ( curr == 100 && ( curr2 == 500 || curr2 == 1000 ) ) ) {
                    // Subtract from next.
                    result -= curr;
                    curr = curr2;
                    it++;
                }
            }
        }
        result += curr;
    }
    return result;
}

string Cal::make_key( const string& str )
{
    string key;
    for( string::const_iterator it = str.begin() ; it != str.end() ; it++ ) {
        if( *it != ' ' ) {
            key += *it;
        }
    }
    return Utf8api::normal( key );
}

// Parse a string formated as 'scheme:format' into scheme and format.
bool Cal::split_code( string* scheme, string* format, const string& codes )
{
    assert( scheme != NULL );
    assert( format != NULL );

    size_t pos = codes.find( ':' );
    if( pos != string::npos ) {
        *scheme = codes.substr( 0, pos );
        *format = codes.substr( pos + 1 );
    } else {
        *scheme = codes;
    }
    return true;
}

namespace {
    enum CastType { CT_date, CT_record, CT_none };

    string create_date_str( string& sig, string& date, CastType& ct )
    {
        string d = full_trim( date );
        if ( !d.empty() ) {
            string cast;
            switch ( ct )
            {
            case CT_date:
                d = "date" + sig + "\"" + d + "\"";
                break;
            case CT_record:
                d = "record" + sig + "\"" + d + "\"";
                break;
            case CT_none:
                break;
            }
        }
        date.clear();
        sig.clear();
        ct = CT_date;
        return d;
    }
}

// Convert a date expression string to a script string.
string Cal::parse_date_expr( const string& str )
{
    string script, date, sig;
    string::const_iterator it, nit;
    CastType ct = CT_date;
    for( it = str.begin() ; it != str.end() ; it++ ) {
        switch( *it )
        {
        case '"':
            it++;
            while( it != str.end() && *it != '"' ) {
                date += *it;
                it++;
            }
            if( it != str.end() ) {
                it++;
            }
            break;
        case '[':  // Ignore comments in square brackets.
            for( int cnt = 1 ; it != str.end() && cnt != 0 ; it++ ) {
                if( *it == ']' ) {
                    --cnt;
                } else if( *it == '[' ) {
                    cnt++;
                }
            }
            break;
        case '&': case '-': case '/': // Must be followed by dot.
            nit = it+1;
            if( nit != str.end() && ( *nit == '.' ) ) {
                script += create_date_str( sig, date, ct );
                script += *it; // It's an operator.
                it++;       // Step over dot.
            } else {
                date += *it; // Treat & as part of date string.
            }
            break;
        case '|': case '(': case ')': case '~': // Always recognised operators.
        case '\\': case '^': case '!': case '+': case '*':
            script += create_date_str( sig, date, ct );
            script += *it;
            break;
        case ';': // Use instead of comma.
            script += create_date_str( sig, date, ct );
            script += ',';
            break;
        case '#':
            nit = it + 1;
            if ( nit != str.end() && ( *nit == '#' ) ) {
                // Use ## to mean record cast
                ct = CT_record;
            }
            date = full_trim( date );
            if ( date.empty() ) {
                if ( ct == CT_date ) {
                    ct = CT_none;
                }
            } else {
                sig = ",\"" + date + "\" ";
                date.clear();
            }
            break;
        case '@':
            // TODO: date should be empty at this point, do error checks? 
            for ( ; it != str.end() ; it++ ) {
                date += *it;
                nit = it + 1;
                if ( *nit == '(' ) {
                    break;
                }
            }
            break;
        default:
            date += *it;
        }
        if( it == str.end() ) {
            break;
        }
    }
    script += create_date_str( sig, date, ct );
    return script;
}

// End of src/cal/calparse.cpp file
