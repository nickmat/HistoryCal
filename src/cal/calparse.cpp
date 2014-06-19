/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calparse.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Functions to help parse scripts.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
 * Copyright:   Copyright (c) 2013-2014, Nick Matthews.
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

#include <utf8/utf8api.h>

#include <sstream>

using namespace Cal;
using namespace std;

// Splits into statements that end with the ';' character.
// Removes line comments that start with "//". Leaves the new line.  
// Passes over matched curly brackets.
StringVec Cal::parse_statements( const string& str )
{
    StringVec result;
    string statement;
    int cbracket = 0;
    bool comment = false;
    for( string::const_iterator it = str.begin() ; it != str.end() ; it++ ) {
        if( comment ) {
            if( *it == '\n' ) {
                comment = false;
                statement += ' ';
            }
            continue;
        }
        if( *it == '/' ) {
            if( it+1 != str.end() && *(it+1) == '/' ) {
                comment = true;
                continue;
            }
        } else if( *it == '{' ) {
            cbracket++;
        } else if( *it == '}' ) {
            --cbracket;
        }
        if( cbracket == 0 && *it == ';' ) {
            result.push_back( left_trim( statement ) );
            statement.clear();
        } else {
            if( *it == '\n' || *it == '\t' ) {
                statement += ' ';
            } else {
                statement += *it;
            }
        }
    }
    return result;
}

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

string Cal::get_next_phrase( const string& str, string* tail, char sep )
{
    if( str.empty() ) {
        return "";
    }
    if( *str.begin() != '"' ) {
        return get_first_word( str, tail, sep );
    }
    size_t pos = str.find( '"', 1 );
    string result = str.substr( 1, pos - 1 );
    if( tail ) {
        *tail = ( pos == string::npos ) ? "" : left_trim( str.substr( pos + 1 ) );
    }
    return result;
}

string Cal::peel_cbrackets( const string& str )
{
    string result;
    int cbracket = 0;
    for( string::const_iterator it = str.begin() ; it != str.end() ; it++ ) {
        if( *it == '{' ) {
            cbracket++;
            if( cbracket == 1 ) {
                continue;
            }
        }
        if( *it == '}' ) {
            --cbracket;
            if( cbracket <= 0 ) {
                break;
            }
        }
        if( cbracket > 0 ) {
            result += *it;
        }
    }
    return left_trim( result );
}

string Cal::field_to_str( Field field )
{
    if( field== f_invalid ) {
        return "";
    }
    if( field == f_maximum ) {
        return "future.";
    }
    if( field == f_minimum ) {
        return "past.";
    }
    ostringstream ss; 
    ss << field; 
    return ss.str();
}

Field Cal::str_to_field( const std::string& str )
{
    return strtol( str.c_str(), NULL, 10 );
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

// Convert a date expression string to a script string.
string Cal::parse_date_expr( const string& str )
{
    string script, date;
    string::const_iterator it, nit;
    for( it = str.begin() ; it != str.end() ; it++ ) {
        switch( *it )
        {
        case '[':  // Ignore comments in square brackets.
            for( int cnt = 1 ; it != str.end() && cnt != 0 ; it++ ) {
                if( *it == ']' ) {
                    --cnt;
                } else if( *it == '[' ) {
                    cnt++;
                }
            }
        case '&':
            nit = it+1;
            if( nit != str.end() && ( *nit == '.' ) ) {
                date = full_trim( date );
                if( date.size() ) {
                    script += "L\"" + date + "\"";
                    date.clear();
                }
                script += *it;
                it++;       // Step over dot, next char is treated as operator.
            } else {
                date += *it; // Treat dot as part of date string.
            }
            break;
        case '|': case '\\': case '^': case '!': case '(': case ')':
            date = full_trim( date );
            if( date.size() ) {
                script += "L\"" + date + "\"";
                date.clear();
            }
            script += *it;
            break;
        default:
            date += *it;
            break;
        }
        if( it == str.end() ) {
            break;
        }
    }
    date = full_trim( date );
    if( date.size() ) {
        script += "L\"" + date + "\"";
    }
    return script;
}

// End of src/cal/calparse.cpp file
