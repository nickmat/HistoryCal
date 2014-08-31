/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscript.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Run script.
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

#include <cassert>

using namespace std;
using namespace Cal;


Script::Script( Calendars* cals, istream& in, ostream& out ) 
    : m_cals(cals), m_ts(in,out), m_out(&out), m_err(&out)
{
    assert( cals != NULL );
}

bool Script::run()
{
    bool ret = false;
    STokenStream* prev_ts = SValue::set_token_stream( &m_ts );
    for(;;) {
        m_ts.next();
        if( m_ts.current().type() == SToken::STT_End ) {
            ret = true;
            break;
        }
        if( statement() == false ) {
            ret = ( m_ts.errors() == 0 );
            break;
        }
    }
    SValue::set_token_stream( prev_ts );
    return ret;
}

ScriptStore* Script::store() const
{
    return m_cals->get_store();
}

bool Script::statement()
{
    SToken token = m_ts.current();

    if( token.type() == SToken::STT_Name ) {
        string name = token.get_str();
        if( name == "end" ) return false;
        if( name == "set" ) return do_set();
        if( name == "let" ) return do_let();
        if( name == "write" ) return do_write();
        if( name == "writeln" ) return do_writeln();
        if( name == "scheme" ) return do_scheme();
        if( name == "vocab" ) return do_vocab();
        if( name == "grammar" ) return do_grammar();
    } else if( token.type() == SToken::STT_Semicolon ) {
        return true; // Empty statement
    }
    // Assume error
    return error( "Unrecognised statement." ); 
}

bool Script::do_set()
{
    string prop;
    SToken token = m_ts.next();
    if( token.type() == SToken::STT_Name ) {
        prop = token.get_str();
        token = m_ts.next();
    }
    string value;
    if( token.type() == SToken::STT_Name || token.type() == SToken::STT_String ) {
        value = token.get_str();
        token = m_ts.next();
    }
    if( token.type() != SToken::STT_Semicolon ) {
        error( "set statement is \"set propery value;\"." );
        return false;
    }
    SHandle sch = m_cals->get_scheme( value );
    if( sch ) {
        if( prop == "input" ) {
            store()->ischeme = sch;
        } else if( prop == "output" ) {
            store()->oscheme = sch;
        } else {
            error( "Set property \"" + prop + "\" not recognised." );
        }
    } else {
        error( "Set value \"" + value + "\" not recognised." );
    }
    return true;
}

bool Script::do_let()
{
    string var;
    SToken token = m_ts.next();
    if( token.type() == SToken::STT_Name ) {
        var = token.get_str();
        token = m_ts.next();
    }
    if( token.type() != SToken::STT_Equal ) {
        error( "'=' expected." );
        return false;
    }
    SValue value = expr( true );
    store()->table[var] = value;
    token = m_ts.current();
    if( token.type() != SToken::STT_Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool Script::do_write()
{
    SValue value = expr( true );
    string str;
    if( value.get( str ) ) {
        *m_out << str;
    } else {
        error( "Unable to output string" );
        return false;
    }
    if( m_ts.current().type() != SToken::STT_Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool Script::do_writeln()
{
    bool ret = do_write();
    *m_out << "\n";
    return ret;
}

bool Script::do_scheme()
{
    string code = m_ts.read_function();
    if( code.size() ) {
        m_cals->add_scheme( code );
        return true;
    }
    return false;
}

bool Script::do_vocab()
{
    string code = m_ts.read_function();
    if( code.size() ) {
        m_cals->add_vocab( code );
        return true;
    }
    return false;
}

bool Script::do_grammar()
{
    string code;
    SToken token = m_ts.next();
    if( token.type() == SToken::STT_Name || token.type() == SToken::STT_String ) {
        code = token.get_str();
    } else {
        error( "Grammar code missing." );
        return false;
    }
    Grammar* gmr = m_cals->add_grammar( code );
    for(;;) {
        token = m_ts.next();
        if( token.type() == SToken::STT_LCbracket ) {
            continue; 
        } else if( token.type() == SToken::STT_RCbracket ) {
            break; // All done.
        } else if( token.type() == SToken::STT_Name ) {
            string name = token.get_str();
            if( name == "vocabs" ) {
                gmr->add_vocabs( m_cals, m_ts.read_function() );
            } else if( name == "format" ) {
                gmr->add_format( m_ts.read_function() );
            } else if( name == "alias" ) {
                gmr->add_alias( m_ts.read_function() );
            } else if( name == "grammar" ) {
                gmr->set_inherit( m_cals, m_ts.read_function() );
            } else {
                error( "Unknown grammar subcommand." );
                m_ts.read_function();
            }
        }
    }
    return true;
}

SValue Script::expr( bool get )
{
    SValue left = compare( get );
    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_or:
            left.logical_or( compare( true ) );
            break;
        case SToken::STT_and:
            left.logical_and( compare( true ) );
        default:
            return left;
        }
    }
}

SValue Script::compare( bool get )
{
    SValue left = combine( get );
    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Equal:
            left.equal( combine( true ) );
            break;
        case SToken::STT_NotEqual:
            left.equal( combine( true ) );
            left.logical_not();
            break;
        case SToken::STT_GtThan:
            left.greater_than( combine( true ) );
            break;
        case SToken::STT_GtThanEq:
            left.less_than( combine( true ) );
            left.logical_not();
            break;
        case SToken::STT_LessThan:
            left.less_than( combine( true ) );
            break;
        case SToken::STT_LessThanEq:
            left.greater_than( combine( true ) );
            left.logical_not();
            break;
        default:
            return left;
        }
    }
}

SValue Script::combine( bool get )
{
    SValue left = range( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_UNION:
            left.rlist_union( range( true ) );
            break;
        case SToken::STT_INTERSECTION:
            left.intersection( range( true ) );
            break;
        case SToken::STT_REL_COMPLEMENT:
            left.rel_complement( range( true ) );
            break;
        case SToken::STT_SYM_DIFFERENCE:
            left.sym_difference( range( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::range( bool get )
{
    SValue left = sum( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Tilde:
            left.range_op( sum( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::sum( bool get )
{
    SValue left = term( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Plus:
            left.plus( term( true ) );
            break;
        case SToken::STT_Minus:
            left.minus( term( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::term( bool get )
{
    SValue left = primary( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Star:
            left.multiply( primary( true ) );
            break;
        case SToken::STT_Divide:
            left.divide( primary( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::primary( bool get )
{
    SValue value;
    SToken token = get ? m_ts.next() : m_ts.current();
    switch( token.type() )
    {
    case SToken::STT_Number:
        value.set_field( token.get_field() );
        m_ts.next();
        break;
    case SToken::STT_Qmark:
        value.set_field( f_invalid );
        m_ts.next();
        break;
    case SToken::STT_String:
        value.set_str( token.get_str() );
        m_ts.next();
        break;
    case SToken::STT_Name:
        value = get_value_var( token.get_str() );
        m_ts.next();
        break;
    case SToken::STT_Lbracket:
        value = expr( true );
        if( m_ts.current().type() != SToken::STT_Rbracket ) {
            error( "')' expected." );
            break;
        }
        m_ts.next();
        break;
    case SToken::STT_LCbracket:
        value = fields_expr( true );
        m_ts.next();
        break;
    case SToken::STT_date:
        {
            value = primary( true );
            SHandle sch = store()->ischeme;
            value.set( m_cals->str_to_rangelist( sch, value.get_str() ) );
        }
        break;
    case SToken::STT_mask:
        {
            value = primary( true );
            SHandle sch = store()->ischeme;
            value.set_fields( m_cals->str_to_fieldvec( sch, value.get_str() ) );
        }
        break;
    case SToken::STT_str_cast:
        value = primary( true );
        {
            SHandle sch = store()->oscheme;
            if( sch ) {
                RangeList rlist;
                value.get_rlist( rlist );
                value.set_str( sch->rangelist_to_str( rlist ) );
            } else {
                error( "Valid scheme not set." );
            }
        }
        break;
    case SToken::STT_Minus:
        value = primary( true );
        value.negate();
        break;
    case SToken::STT_not:
        value = primary( true );
        value.logical_not();
        break;
    case SToken::STT_COMPLEMENT:
        value = primary( true );
        value.compliment();
        break;
    default:
        error( "Primary value expected." );
    }
    return value;
}

SValue Script::fields_expr( bool get )
{
    FieldVec fields;
    SToken token = get ? m_ts.next() : m_ts.current();

    for(;;) {
        switch( token.type() )
        {
        case SToken::STT_End:
        case SToken::STT_RCbracket:
            return SValue( fields );
        case SToken::STT_Comma:
            break;
        default:
            {
                SValue value = expr( false );
                Field field = value.get_field();
                if( value.is_error() ) {
                    error( "Field value expected." );
                } else {
                    fields.push_back( field );
                }
            }
            token = m_ts.current();
            continue;
        }
        token = m_ts.next();
    }
}

SValue Script::get_value_var( const string& name )
{
    if( name == "true" ) {
        return SValue( true );
    }
    if( name == "false" ) {
        return SValue( false );
    }
    if( name == "invalid" ) {
        return SValue( f_invalid );
    }
    if( name == "past" ) {
        return SValue( f_minimum );
    }
    if( name == "future" ) {
        return SValue( f_maximum );
    }
    if( store()->table.count( name ) ) {
        return store()->table.find( name )->second;
    }
    SValue value;
    value.set_error( "Variable \"" + name + "\" not found." );
    return value;
}

// End of src/cal/calscript.cpp file
