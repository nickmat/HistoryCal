/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscript.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Evaluate script.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     7th May 2014
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

#include "calscript.h"

#include "calparse.h"
#include "calscheme.h"
#include "calschemes.h"
#include "calsetmath.h"

#include <cassert>

using namespace std;
using namespace Cal;

Script::Script( Schemes* schs )
    : m_schemes(schs), m_line(0), m_mode(MODE_Normal)
{
}

bool Script::run( const string& script )
{
    m_it = script.begin();
    m_end = script.end();
    m_line = 1;
    m_output.clear();
    get_token();
    for(;;) {
        switch( m_cur_token )
        {
        case ST_Semicolon:
            get_token();  // Ignore empty statements
            break;
        case ST_End:
            return true;
        case ST_date:
            do_date();
            break;
        case ST_set:
            do_set();
            break;
        case ST_evaluate:
            do_evaluate();
            break;
        case ST_write:
            m_output += string_expr();
            break;
        case ST_writeln:
            m_output += string_expr() + '\n';
            break;
        case ST_vocab:
            m_schemes->add_vocab( read_function() );
            break;
        case ST_scheme:
            m_schemes->add_scheme( read_function() );
            break;
        case ST_grammar:
            m_schemes->add_grammar( read_function() );
            break;
        default:
            get_token(); // TODO: error, but we need to move on
            break;
        }
    }
    return true;
}


void Script::do_date()
{
    if( get_token() != ST_Name ) {
        return; // TODO: syntax error
    }
    string name = m_cur_name;
    if( get_token() != ST_Equals ) {
        return; // TODO: syntax error
    }
    RangeList rlist = date_expr();
    m_schemes->store()->rlisttable[name] = rlist;
}

RangeList Script::date_expr()
{
    RangeList left = date_value();
    get_token();
    for(;;) {
        switch( m_cur_token )
        {
        case ST_UNION:
            left = op_set_union( left, date_value() );
            get_token();
            break;
        case ST_INTERSECTION:
            left = op_set_intersection( left, date_value() );
            get_token();
            break;
        case ST_REL_COMPLEMENT:
            left = op_set_rel_complement( left, date_value() );
            get_token();
            break;
        case ST_SYM_DIFFERENCE:
            left = op_set_sim_difference( left, date_value() );
            get_token();
            break;
        default:
            return left;
        }
    }
    return left;
}

RangeList Script::date_value()
{
    RangeList rlist;
    Range range;
    Scheme* sch;
    get_token();
    switch( m_cur_token )
    {
    case ST_Number:
        range.jdn1 = range.jdn2 = m_cur_number;
        rlist.push_back( range );
        break;
    case ST_String:
        sch = m_schemes->store()->ischeme;
        if( sch ) {
            rlist = sch->r_str_to_rangelist( m_cur_text );
        }
        break;
    case ST_Name:
        rlist = get_rlist_name( m_cur_name );
        break;
    case ST_Lbracket:
        rlist = date_expr();
        if( m_cur_token != ST_Rbracket ) {
            // TODO: error ')' expected
            break;
        }
        break;
    case ST_COMPLEMENT:
        rlist = op_set_complement( date_value() );
        break;
    default:
        // TODO: error RangeList expected
        break;
    }
    return rlist;
}

void Script::do_set()
{
    SToken prop = get_token();
    if( prop == ST_Semicolon ) {
        return;
    }
    SToken tvalue = get_token();
    if( tvalue != ST_String ) {
        return;
    }
    Scheme* sch = m_schemes->get_scheme( m_cur_text );
    if( sch ) {
        if( prop == ST_input ) {
            m_schemes->store()->ischeme = sch;
        } else if( prop == ST_output ) {
            m_schemes->store()->oscheme = sch;
        }
    }
    get_token(); // TODO: error if this is not ST_Semicolon
}

void Script::do_evaluate()
{
    switch( look_next_token() )
    {
    case ST_date:
        get_token();
        m_date_out = date_expr();
        break;
    default:
        break;
    }
}

string Script::string_expr()
{
    string left = string_value();
    get_token();
    for(;;) {
        switch( m_cur_token )
        {
        case ST_Plus:
            left += string_value();
            get_token();
            break;
        default:
            return left;
        }
    }
}

string Script::string_value()
{
    string str;
    RangeList rlist;
    Scheme* sch;
    get_token();
    switch( m_cur_token )
    {
    case ST_Number:
        str = field_to_str( m_cur_number );
        break;
    case ST_String:
        str = m_cur_text;
        break;
    case ST_Name:
        rlist = get_rlist_name( m_cur_name );
        sch = m_schemes->store()->oscheme;
        if( sch ) {
            str = sch->rangelist_to_str( rlist );
        }
        break;
    case ST_date:
        get_token();
        rlist = date_expr();
        sch = m_schemes->store()->oscheme;
        if( sch ) {
            str = sch->rangelist_to_str( rlist );
        }
        break;
    default:
        // TODO: error ? expected
        break;
    }
    return str;
}

Script::SToken Script::get_token()
{
    // TODO: handle utf-8 unicode
    if( m_it == m_end ) {
        return m_cur_token = ST_End;
    }
    SToken token = ST_Null;
    string text;
    string::const_iterator it = m_it;
    for( ; it != m_end ; it++ ) {
        if( *it == ' ' || *it == '\t' ) {
            continue;
        }
        if( *it == '\n' ) {
            m_line++;
            continue;
        }
        if( isdigit( *it ) ) {
            token = ST_Number;
            do {
                text += *it++;
            } while( it != m_end && isdigit( *it ) );
            m_cur_number = str_to_field( text );
            break;
        }
        if( *it == '"' ) {
            m_cur_text.clear();
            token = ST_String;
            it++;
            while( it != m_end && *it != '"' ) {
                m_cur_text += *it++;
            }
            if( *it == '"' ) {
                it++; // step over trailing quote
            }
            break;
        }
        if( isalpha( *it ) ) {
            token = ST_Name;
            do {
              text += *it++;
            } while( it != m_end && isalnum( *it ) );
            if( text == "date" ) {
                token = ST_date;
            } else if( text == "output" ) {
                token = ST_output;
            } else if( text == "input" ) {
                token = ST_input;
            } else if( text == "set" ) {
                token = ST_set;
            } else if( text == "evaluate" ) {
                token = ST_evaluate;
            } else if( text == "write" ) {
                token = ST_write;
            } else if( text == "writeln" ) {
                token = ST_writeln;
            } else if( text == "vocab" ) {
                token = ST_vocab;
            } else if( text == "scheme" ) {
                token = ST_scheme;
            } else if( text == "grammar" ) {
                token = ST_grammar;
            }
            if( token == ST_Name ) {
                m_cur_name = text;
            }
            break;
        }
        switch( *it )
        {
        case '=': token = ST_Equals; break;
        case '+': token = ST_Plus; break;
        case '-': token = ST_Minus; break;
        case '/': token = ST_Divide; break;
        case '*': token = ST_Star; break;
        case '%': token = ST_Percent; break;
        case '(': token = ST_Lbracket; break;
        case ')': token = ST_Rbracket; break;
        case '{': token = ST_LCbracket; break;
        case '}': token = ST_RCbracket; break;
        case '[': token = ST_LSbracket; break;
        case ']': token = ST_RSbracket; break;
        case ';': token = ST_Semicolon; break;
        case '|': token = ST_Vline; break;
        case '&': token = ST_Ampersand; break;
        case '!': token = ST_Exclamation; break;
        case '^': token = ST_Carrot; break;
        case '\\': token = ST_Backslash; break;
        }
        if( token != ST_Null ) {
            it++;
            break;
        }
    }
    m_it = it;
    m_cur_token = token;
    return token;
}

Script::SToken Script::look_next_token()
{
    string::const_iterator position = m_it;
    SToken token = get_token();
    m_it = position;
    return token;
}

RangeList Script::get_rlist_name( const string& name ) const
{
    RangeList rlist;
    if( m_schemes->store()->rlisttable.count( name ) ) {
        rlist = m_schemes->store()->rlisttable.find( name )->second;
    } else {
        // TODO: error name not found
    }
    return rlist;
}

string Script::read_function()
{
    string def;
    int bcount = 0;
    while( m_it != m_end && isspace( *m_it ) ) {
        m_it++; // remove leading whitespace
    }
    while( m_it != m_end ) {
        def += *m_it;
        if( *m_it == '{' ) {
            bcount++;
        } else if( *m_it == '}' ) {
            if( bcount == 1 ) {
                m_it++;
                break;
            }
            --bcount;
        }
        m_it++;
    }
    get_token();
    return def;
}


// End of src/cal/calscript.cpp file
