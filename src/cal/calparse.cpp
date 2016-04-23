/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calparse.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Functions to help parse scripts.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
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
    string create_date_str( string& sig, string& date )
    {
        string d = full_trim( date );
        if( !d.empty() ) {
            d = "date" + sig + "\"" + d + "\"";
            date.clear();
        }
        sig.clear();
        return d;
    }
}

// Convert a date expression string to a script string.
string Cal::parse_date_expr( const string& str )
{
    string script, date, sig;
    string::const_iterator it, nit;
    for( it = str.begin() ; it != str.end() ; it++ ) {
        switch( *it )
        {
        case '"':
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
        case '&':
            nit = it+1;
            if( nit != str.end() && ( *nit == '.' ) ) {
                script += create_date_str( sig, date );
                script += *it;
                it++;       // Step over dot, next char is treated as operator.
            } else {
                date += *it; // Treat & as part of date string.
            }
            break;
        case '|': case '\\': case '^': case '!': case '(': case ')': case '~':
            script += create_date_str( sig, date );
            script += *it;
            break;
        case '#':
            date = full_trim( date );
            if( !date.empty() ) {
                sig = ",\"" + date + "\" ";
                date.clear();
            }
            break;
        default:
            date += *it;
            break;
        }
        if( it == str.end() ) {
            break;
        }
    }
    script += create_date_str( sig, date );
    return script;
}

// End of src/cal/calparse.cpp file
