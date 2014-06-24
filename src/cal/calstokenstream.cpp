/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calstokenstream.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Translate a character stream to a token stream.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     24th June 2014
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

#include "calstokenstream.h"

#include "calparse.h"

using namespace std;
using namespace Cal;


#define MAX_ALLOWED_ERRORS 5

SToken STokenStream::next()
{
    char ch;
    SToken::Type str_token = SToken::STT_String;
    bool lcomment = false, mcomment = false, inquote = false;

    do { // Skip whitespace.
        if( !m_in->get( ch ) ) {
            set_type( SToken::STT_End );
            return m_cur_token;
        }
        if( ch == '"' ) {
            inquote = !inquote;
        }
        if( !inquote ) {
            if( ch == '/' && m_in->peek() == '*' ) {
                mcomment = true;
            }
            if( ch == '*' && m_in->peek() == '/' ) {
                m_in->get( ch );
                m_in->get( ch ); 
                mcomment = false;
            }
            if( ch == '/' && m_in->peek() == '/' ) {
                lcomment = true;
            }
        }
        if( ch == '\n' ) {
            m_line++;
            lcomment = false;
        }
    } while( isspace( ch ) || lcomment || mcomment );

    if( isdigit( ch ) ) {
        string text;
        do {
            text += ch;
        } while( m_in->get( ch ) && isdigit( ch ) );
        m_in->putback( ch );
        set_current( SToken::STT_Number, str_to_field( text ) );
        return m_cur_token;
    }

    // TODO: This should recognise a utf-8 alpha codepoint
    string str;
    if( isalpha( ch ) ) {
        str += ch;
        while( m_in->get( ch ) && isalnum( ch ) ) {
            str += ch;
        }
        m_in->putback( ch );
        if( str == "or" ) {
            set_type( SToken::STT_or );
        } else if( str == "and" ) {
            set_type( SToken::STT_and );
        } else if( str == "not" ) {
            set_type( SToken::STT_not );
        } else if( str == "str" ) {
            set_type( SToken::STT_str_cast );
        } else if( str == "date" ) {
            set_type( SToken::STT_date );
        } else {
            set_current( SToken::STT_Name, str );
        }
        return m_cur_token;
    }

    if( ch == '"' ) {
        string text;
        while( m_in->get( ch ) && ch != '"' ) {
            text += ch;
        }
        set_current( str_token, text );
        return m_cur_token;
    }

    switch( ch )
    {
    case '=': set_type( SToken::STT_Equal ); break;
    case '+': set_type( SToken::STT_Plus ); break;
    case '-': set_type( SToken::STT_Minus ); break;
    case '/': set_type( SToken::STT_Divide ); break;
    case '*': set_type( SToken::STT_Star ); break;
    case '%': set_type( SToken::STT_Percent ); break;
    case '(': set_type( SToken::STT_Lbracket ); break;
    case ')': set_type( SToken::STT_Rbracket ); break;
    case '{': set_type( SToken::STT_LCbracket ); break;
    case '}': set_type( SToken::STT_RCbracket ); break;
    case '[': set_type( SToken::STT_LSbracket ); break;
    case ']': set_type( SToken::STT_RSbracket ); break;
    case ';': set_type( SToken::STT_Semicolon ); break;
    case '~': set_type( SToken::STT_Tilde ); break;
    case '|': set_type( SToken::STT_Vline ); break;
    case '&': set_type( SToken::STT_Ampersand ); break;
    case '!': set_type( SToken::STT_Exclamation ); break;
    case '^': set_type( SToken::STT_Carrot ); break;
    case '\\': set_type( SToken::STT_Backslash ); break;
    default:
        error( "Unrecognised token." );
        set_type( SToken::STT_End );
        break; // Error
    }
    return m_cur_token;
}

string STokenStream::read_function()
{
    char ch;
    string def;

    do { // Skip whitespace.
        if( !m_in->get( ch ) ) {
            return "";
        }
        if( ch == '\n' ) {
            m_line++;
        }
    } while( isspace( ch ) );

    def += ch;
    while( m_in->get( ch ) && ch != '{' && ch != ';' ) {
        def += ch;
    }
    if( ch == '{' ) {
        int bcount = 0;
        def += ch;
        while( m_in->get( ch ) ) {
            def += ch;
            if( ch == '{' ) {
                bcount++;
            } else if( ch == '}' ) {
                if( bcount == 0 ) break;
                --bcount;
            }
        }
    }
    return def;
}

bool STokenStream::error( const string& mess )
{
    *m_err << "Error (" << m_line << "): " << mess << "\n";
    return ( ++m_errors > MAX_ALLOWED_ERRORS );
}

void STokenStream::set_current( SToken::Type type, const std::string& str )
{
    m_cur_token.set_type( type );
    m_cur_token.set_value( str );
}

void STokenStream::set_current( SToken::Type type, Field num )
{
    m_cur_token.set_type( type );
    m_cur_token.set_value( num );
}

// End of src/cal/calstokenstream.cpp file
