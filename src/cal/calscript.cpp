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

#include "cal/calendars.h"
#include "calgrammar.h"
#include "calparse.h"
#include "calscheme.h"
#include "calschemes.h"
#include "calsetmath.h"

#include <cassert>

using namespace std;
using namespace Cal;

Script::Script( Calendars* cals )
    : m_calendars(cals), m_schemes(NULL), m_line(0), m_mode(MODE_Normal)
{
}

Script::Script( Schemes* schs )
    : m_calendars(NULL), m_schemes(schs), m_line(0), m_mode(MODE_Normal)
{
}

bool Script::run( const string& script )
{
    m_it = script.begin();
    m_end = script.end();
    m_line = 1;
    m_output.clear();
    for(;;) {
        switch( get_token() )
        {
        case ST_Semicolon:
            break;  // Ignore empty statements
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
            do_vocab( read_function() );
            break;
        case ST_scheme:
            do_scheme( read_function() );
            break;
        case ST_grammar:
            do_grammar();
            break;
        default:
            break; // TODO: error, but we need to move on
        }
    }
    return true;
}

ScriptStore* Script::get_store() const
{
    if( m_calendars ) {
        return m_calendars->get_store();
    } else if( m_schemes ) {
        return m_schemes->store();
    }
    return NULL;
}

Scheme* Script::get_scheme( const string& code ) const
{
    if( m_calendars ) {
        return m_calendars->get_scheme( code );
    } else if( m_schemes ) {
        return m_schemes->get_scheme( code );
    }
    return NULL;
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
    get_store()->rlisttable[name] = rlist;
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
        sch = get_store()->ischeme;
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
    if( prop != ST_Name ) {
        return;
    }
    string prop_str = m_cur_name;
    SToken tvalue = get_token();
    if( tvalue != ST_String ) {
        return;
    }
    Scheme* sch = get_scheme( m_cur_text );
    if( sch ) {
        if( prop_str == "input" ) {
            get_store()->ischeme = sch;
        } else if( prop_str == "output" ) {
            get_store()->oscheme = sch;
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

void Script::do_vocab( const string& code )
{
    if( m_calendars ) {
        m_calendars->add_vocab( code );
    } else if( m_schemes ) {
        m_schemes->add_vocab( code );
    }
}

void Script::do_grammar()
{
    string code;
    switch( get_token() )
    {
    case ST_Name:
        code = m_cur_name;
        break;
    case ST_String:
        code = m_cur_text;
        break;
    default:
        // ERROR:
        return;
    }
    Grammar* gmr = NULL;
    if( m_calendars ) {
        gmr = m_calendars->add_grammar( code );
    } else {
        gmr = m_schemes->add_grammar( code );
    }
    if( get_token() != ST_LCbracket ) {
        // ERROR: expecting '{'
        return;
    }
    for(;;) {
        SToken token = get_token();
        if( m_cur_token == ST_RCbracket ) {
            break; // All done.
        } else if( m_cur_name == "vocabs" ) {
            if( m_schemes ) {
                gmr->add_vocabs( m_schemes, read_to_semicolon() );
            } else {
                gmr->add_vocabs( m_calendars, read_to_semicolon() );
            }
        } else if( m_cur_name == "format" ) {
            gmr->add_format( read_to_semicolon() );
        } else if( m_cur_name == "alias" ) {
            gmr->add_alias( read_function() );
        } else if( m_cur_name == "grammar" ) {
            if( m_schemes ) {
                gmr->set_inherit( m_schemes, read_to_semicolon() );
            } else {
                gmr->set_inherit( m_calendars, read_to_semicolon() );
            }
        } else {
            // ERROR:
            read_to_semicolon();
        }
        if( get_token() != ST_Semicolon ) {
            // ERROR:
            return;
        }
    }
}

void Script::do_scheme( const string& code )
{
    if( m_calendars ) {
        m_calendars->add_scheme( code );
    } else if( m_schemes ) {
        m_schemes->add_scheme( code );
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
        sch = get_store()->oscheme;
        if( sch ) {
            str = sch->rangelist_to_str( rlist );
        }
        break;
    case ST_date:
        get_token();
        rlist = date_expr();
        sch = get_store()->oscheme;
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
    string::const_iterator it = m_it;
    // Ignore whitespace and comments
    for(;;) {
        if( it == m_end ) {
            m_it = it;
            return m_cur_token = ST_End;
        }
        if( *it == ' ' || *it == '\t' || *it == '\n' ) {
            if( *it == '\n' ) {
                m_line++;
            }
            it++;
            continue;
        }
        if( *it == '/' && (it+1) != m_end && *(it+1) == '/' ) {
            it += 2;
            while( it != m_end && *it != '\n' ) {
                it++;
            }
            continue;
        }
        break;
    }
    SToken token = ST_Null;
    for( ; it != m_end ; it++ ) {
        if( isdigit( *it ) ) {
            string text;
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
            m_cur_name.clear();
            do {
              m_cur_name += *it++;
            } while( it != m_end && isalnum( *it ) );
            if( m_cur_name == "date" ) {
                token = ST_date;
            } else if( m_cur_name == "set" ) {
                token = ST_set;
            } else if( m_cur_name == "evaluate" ) {
                token = ST_evaluate;
            } else if( m_cur_name == "write" ) {
                token = ST_write;
            } else if( m_cur_name == "writeln" ) {
                token = ST_writeln;
            } else if( m_cur_name == "vocab" ) {
                token = ST_vocab;
            } else if( m_cur_name == "scheme" ) {
                token = ST_scheme;
            } else if( m_cur_name == "grammar" ) {
                token = ST_grammar;
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
    if( get_store()->rlisttable.count( name ) ) {
        rlist = get_store()->rlisttable.find( name )->second;
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
    return def;
}

string Script::read_to_semicolon()
{
    string str;
    while( m_it != m_end && isspace( *m_it ) ) {
        m_it++; // remove leading whitespace
    }
    while( m_it != m_end && *m_it != ';' ) {
        str += *m_it;
        m_it++;
    }
    return str;
}

// End of src/cal/calscript.cpp file
