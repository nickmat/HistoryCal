/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calstokenstream.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Translate a character stream to a token stream.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     24th June 2014
 * Copyright:   Copyright (c) 2014 ~ 2020, Nick Matthews.
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
    if( isascii( ch ) && ( isalpha( ch ) || ch == '_' || ch == ':' ) ) {
        str += ch;
        while( m_in->get( ch ) ) {
            if( isascii( ch ) && ( isalnum( ch ) || ch == '_' || ch == ':' ) )
            {
                str += ch;
            } else {
                break;
            }
        }
        m_in->putback( ch );
        if( str == "or" ) {
            set_type( SToken::STT_or );
        } else if( str == "and" ) {
            set_type( SToken::STT_and );
        } else if( str == "not" ) {
            set_type( SToken::STT_not );
        } else if ( str == "mod" ) {
            set_type( SToken::STT_mod );
        } else if( str == "string" ) {
            set_type( SToken::STT_str_cast );
        } else if( str == "date" ) {
            set_type( SToken::STT_date );
        } else if ( str == "record" ) {
            set_type( SToken::STT_record );
        } else if ( str == "convert" ) {
            set_type( SToken::STT_convert );
        } else if ( str == "error" ) {
            set_type( SToken::STT_error );
        } else {
            set_current( SToken::STT_Name, str );
        }
        return m_cur_token;
    }

    if( ch == '"' ) {
        string text;
        while( m_in->get( ch ) ) {
            if( ch == '"' ) {
                if( m_in->peek() == '"' ) {
                    m_in->get( ch );
                } else {
                    break;
                }
            }
            if( ch == '\n' ) {
                m_line++;
                error( "Newline in string literal." );
                break;
            }
            text += ch;
        }
        set_current( str_token, text );
        return m_cur_token;
    }

    switch( ch )
    {
    case '=': set_type( SToken::STT_Equal ); break;
    case '+':
        switch( m_in->peek() ) {
        case '=': m_in->get( ch ); set_type( SToken::STT_PlusEq ); break;
        default: set_type( SToken::STT_Plus ); break;
        } break;
    case '-':
        switch( m_in->peek() ) {
        case '=': m_in->get( ch ); set_type( SToken::STT_MinusEq ); break;
        default: set_type( SToken::STT_Minus ); break;
        } break;
    case '/':
        switch( m_in->peek() ) {
        case '=': m_in->get( ch ); set_type( SToken::STT_DivideEq ); break;
        default: set_type( SToken::STT_Divide ); break;
        } break;
    case '*':
        switch( m_in->peek() ) {
        case '=': m_in->get( ch ); set_type( SToken::STT_StarEq ); break;
        default: set_type( SToken::STT_Star ); break;
        } break;
    case '%': set_type( SToken::STT_Percent ); break;
    case '<':
        switch( m_in->peek() ) {
        case '>': m_in->get( ch ); set_type( SToken::STT_NotEqual ); break;
        case '=': m_in->get( ch ); set_type( SToken::STT_LessThanEq ); break;
        default: set_type( SToken::STT_LessThan ); break;
        } break;
    case '>':
        switch( m_in->peek() ) {
        case '=': m_in->get( ch ); set_type( SToken::STT_GtThanEq ); break;
        default: set_type( SToken::STT_GtThan ); break;
        } break;
    case '(': set_type( SToken::STT_Lbracket ); break;
    case ')': set_type( SToken::STT_Rbracket ); break;
    case '{': set_type( SToken::STT_LCbracket ); break;
    case '}': set_type( SToken::STT_RCbracket ); break;
    case '[': set_type( SToken::STT_LSbracket ); break;
    case ']': set_type( SToken::STT_RSbracket ); break;
    case ';': set_type( SToken::STT_Semicolon ); break;
    case ',': set_type( SToken::STT_Comma ); break;
    case '.':
        switch ( m_in->peek() ) {
        case '.': m_in->get( ch ); set_type( SToken::STT_DotDot ); break;
        default: set_type( SToken::STT_Dot ); break;
        } break;
    case '~': set_type( SToken::STT_Tilde ); break;
    case '|': set_type( SToken::STT_Vline ); break;
    case '&': set_type( SToken::STT_Ampersand ); break;
    case '!': set_type( SToken::STT_Exclamation ); break;
    case '^': set_type( SToken::STT_Carrot ); break;
    case '\\': set_type( SToken::STT_Backslash ); break;
    case '?': set_type( SToken::STT_Qmark ); break;
    case '@': set_type( SToken::STT_At ); break;
    default:
        error( "Unrecognised token." );
        set_type( SToken::STT_End );
        break; // Error
    }
    return m_cur_token;
}

void Cal::STokenStream::skip_to( SToken::Type type )
{
    while ( m_cur_token.type() != type && m_cur_token.type() != SToken::STT_End ) {
        next();
    }
}

string STokenStream::read_until( const string& name, const string& esc )
{
    string code;
    char ch;
    int count = 0;
    bool braces = ( name == "}" );
    bool terminal = ( name == ";" );
    for(;;) {
        string word;
        while( m_in->get( ch ) && isalnum( ch ) ) {
            word += ch;
        }
        code += word;
        if( ch == '\n' ) {
            m_line++;
        }
        if ( braces ) {
            if ( ch == '{' ) {
                count++;
            }
            if ( ch == '}' ) {
                if ( count == 0 ) {
                    break;
                }
                --count;
            }
        } else if ( terminal && ch == ';' ) {
            break;
        } else if ( !word.empty() ) {
            if( word == esc ) {
                count++;
            }
            if( word == name ) {
                if( count == 0 ) {
                    break;
                }
                --count;
            }
        }
        if( ch == '/' ) {
            if( m_in->peek() == '*' ) { // Multiline comments
                m_in->get(); // Step over '*'
                while( m_in->get( ch ) ) {
                    if( ch == '\n' ) {
                        m_line++;
                        code += '\n'; // Keep line numbers in sync.
                    }
                    if( ch == '*' && m_in->peek() == '/' ) {
                        m_in->get(); // Step over '/'
                        ch = ' '; // Replace comment with a space
                        break;
                    }
                }
            }
            if( m_in->peek() == '/' ) { // Singleline comments
                m_in->get(); // Step over '/'
                while( m_in->get( ch ) ) {
                    if( ch == '\n' ) {
                        m_line++;
                        break;
                    }
                }
            }
        }
        if( ch == '"' ) {
            code += ch;
            while( m_in->get( ch ) && ch != '"' ) {
                code += ch;
            }
        }
        code += ch;
        if ( m_in->eof() ) { // End of stream before name found.
            return "";
        }
    }
    return code;
}

bool STokenStream::error( const string& mess )
{
    *m_err << "Error (" << m_line << "): " << mess << "\n";
    return ( ++m_errors > MAX_ALLOWED_ERRORS );
}

std::istream* STokenStream::reset_in( std::istream* in )
{
    std::istream* prev = m_in;
    m_in = in;
    return prev;
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

void STokenStream::set_current( SToken::Type type, bool b )
{
    m_cur_token.set_type( type );
    m_cur_token.set_value( b );
}

// End of src/cal/calstokenstream.cpp file
