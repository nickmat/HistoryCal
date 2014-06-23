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
#include "calscriptstore.h"
#include "calsetmath.h"

#include <cassert>

using namespace std;
using namespace Cal;

SValue::SValue( const SValue& value )
{
    m_type = value.type();
    switch( m_type )
    {
    case SVT_Error:
    case SVT_Str:
        m_str = value.get_str();
        break;
    case SVT_Bool:
    case SVT_Field:
        m_range.jdn1 = value.get_field();
        break;
    case SVT_Range:
        m_range = value.get_range();
        break;
    case SVT_RList:
        m_rlist = value.get_rlist();
        break;
    }
}

void SValue::set( RangeList& rlist )
{
    if( rlist.size() == 0 ) {
        set_field( f_invalid );
    } else if( rlist.size() == 1 ) {
        Range range = rlist[0];
        if( range.jdn1 == range.jdn2 ) {
            set_field( range.jdn1 );
        } else {
            set_range( range );
        }
    } else {
        set_rlist( rlist );
    }
}

string SValue::get_str() const
{
    assert( m_type == SVT_Str || m_type == SVT_Error );
    return m_str;
}

bool SValue::get( string& str ) const
{
    str.clear();
    switch( m_type )
    {
    case SVT_Str:
        str = m_str;
        return true;
    case SVT_Bool:
        str = get_bool() ? "true" : "false";
        return true;
    case SVT_Field:
        str = field_to_str( m_range.jdn1 );
        return true;
    case SVT_Range:
        str = field_to_str( m_range.jdn1 );
        if( m_range.jdn1 != m_range.jdn2 ) {
            str += " ~ " + field_to_str( m_range.jdn2 );
        }
        return true;
    case SVT_RList:
        for( size_t i = 0 ; i < m_rlist.size() ; i++ ) {
            if( i > 0 ) {
                str += " | ";
            }
            str += field_to_str( m_rlist[i].jdn1 );
            if( m_rlist[i].jdn1 != m_rlist[i].jdn2 ) {
                str += " ~ " + field_to_str( m_rlist[i].jdn2 );
            }
        }
        return true;
    }
    return false;
}

bool SValue::get_bool() const
{
    assert( m_type == SVT_Bool );
    return m_range.jdn1 == 0 ? false : true;
}

Field SValue::get_field() const
{
    assert( m_type == SVT_Field );
    return m_range.jdn1;
}

Range SValue::get_range() const
{
    assert( m_type == SVT_Range );
    return m_range;
}

RangeList SValue::get_rlist() const
{
    assert( m_type == SVT_RList );
    return m_rlist;
}

bool SValue::get_rlist( RangeList& rlist ) const
{
    switch( m_type )
    {
    case SVT_RList:
        rlist = m_rlist;
        return true;
    case SVT_Null:
        rlist.clear();
        return true;
    case SVT_Range:
        rlist.clear();
        if( m_range.jdn1 != f_invalid ) {
            rlist.push_back( m_range );
        }
        return true;
    case SVT_Field:
        rlist.clear();
        if( m_range.jdn1 != f_invalid ) {
            Range range( m_range.jdn1, m_range.jdn1 );
            rlist.push_back( range );
        }
        return true;
    }
    return false;
}

bool SValue::propagate_error( const SValue& value )
{
    if( is_error() ) {
        return true;
    }
    if( value.is_error() ) {
        set_error( value.get_str() );
        return true;
    }
    return false;
}

bool SValue::obtain_rlists( RangeList& left, RangeList& right, const SValue& value )
{
    if( propagate_error( value ) ) {
        return false;
    }
    if( !get_rlist( left ) ) {
        set_error( "Cannot convert left hand to RList." );
        return false;
    }
    if( !value.get_rlist( right ) ) {
        set_error( "Cannot convert right hand to RList." );
        return false;
    }
    return true;
}

void SValue::logical_or( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    set_bool( get_bool() || value.get_bool() );
}

void SValue::logical_and( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    set_bool( get_bool() && value.get_bool() );
}

void SValue::equal( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case SVT_Str:
            result = ( get_str() == value.get_str() );
            break;
        case SVT_Bool:
        case SVT_Field:
            result = ( get_field() == value.get_field() );
            break;
        case SVT_Range:
            {
                Range vr = value.get_range();
                Range tr = get_range();
                result =
                    ( tr.jdn1 == vr.jdn1 ) &&
                    ( tr.jdn2 == vr.jdn2 )
                ;
            }
            break;
        case SVT_RList:
            {
                RangeList vrl = value.get_rlist();
                RangeList trl = get_rlist();
                if( trl.size() == vrl.size() ) { 
                    result = true;
                    for( size_t i = 0 ; i < trl.size() ; i++ ) {
                        if( ( trl[i].jdn1 != vrl[i].jdn1 ) ||
                            ( trl[i].jdn2 != vrl[i].jdn2 )
                        ) {
                            result = false;
                            break;
                        }
                    }
                }
            }
            break;
        default:
            result = true;
        }
        set_bool( result );
        return;
    }
    set_error( "Must compare same types." );
}
    
void SValue::greater_than( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case SVT_Str:
            result = ( get_str().compare( value.get_str() ) > 0 );
            break;
        case SVT_Bool:
            set_error( "Can only compare bool for equal or not equal." );
            return;
        case SVT_Field:
            result = ( get_field() > value.get_field() );
            break;
        case SVT_Range:
        case SVT_RList:
            set_error( "Can only compare a Range for equal or not equal." );
            return;
        default:
            result = true;
        }
        set_bool( result );
        return;
    }
    set_error( "Must compare same types." );
}

void SValue::less_than( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case SVT_Str:
            result = ( get_str().compare( value.get_str() ) < 0 );
            break;
        case SVT_Bool:
            set_error( "Can only compare bool for equal or not equal." );
            return;
        case SVT_Field:
            result = ( get_field() < value.get_field() );
            break;
        case SVT_Range:
        case SVT_RList:
            set_error( "Can only compare a Range for equal or not equal." );
            return;
        default:
            result = true;
        }
        set_bool( result );
        return;
    }
    set_error( "Must compare same types." );
}

void SValue::plus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == SVT_Str || value.type() == SVT_Str ) {
        string str1, str2;
        bool success = get( str1 );
        if( success ) {
            success = value.get( str2 );
        }
        if( success ) {
            set_str( str1 + str2 );
            return;
        }
        set_error( "Unable to add string." );
        return;
    }
    if( type() == SVT_Field || value.type() == SVT_Field ) {
        if( type() == value.type() ) {
            set_field( add( get_field(), value.get_field() ) );
        } else if( type() == SVT_Range ) {
            set_range( add( get_range(), value.get_field() ) );
        } else if( value.type() == SVT_Range ) {
            set_range( add( value.get_range(), get_field() ) );
        } else if( type() == SVT_RList ) {
            set_rlist( add( get_rlist(), value.get_field() ) );
        } else if( value.type() == SVT_RList ) {
            set_rlist( add( value.get_rlist(), get_field() ) );
        } else {
            set_error( "Unable to add number." );
        }
        return;
    }
    if( type() == SVT_Range || value.type() == SVT_Range ) {
        if( type() == value.type() ) {
            set_range( add( get_range(), value.get_range() ) );
        } else if( type() == SVT_RList ) {
            set_rlist( add( get_rlist(), value.get_range() ) );
        } else if( value.type() == SVT_RList ) {
            set_rlist( add( value.get_rlist(), get_range() ) );
        } else {
            set_error( "Unable to add range." );
        }
        return;
    }
    set_error( "Not able to add types." );
}

void SValue::minus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == SVT_Field || value.type() == SVT_Field || 
        type() == SVT_Range || value.type() == SVT_Range
    ) {
        SValue v( value );
        v.negate();
        plus( v );
        return;
    }
    set_error( "Not able to subtract types." );
}

void SValue::multiply( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    // TODO: check for f_invalid etc and for overflow.
    if( m_type == value.type() ) {
        switch( m_type )
        {
        case SVT_Field:
            set_field( get_field() * value.get_field() );
            return;
        default:
           set_error( "Can only multiply numbers." );
           return;
        }
    }
    set_error( "Must subtract the same types." );
}

void SValue::divide( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    // TODO: check for f_invalid etc and for overflow.
    if( m_type == value.type() ) {
        switch( m_type )
        {
        case SVT_Field:
            {
                Field right = value.get_field();
                if( right == 0 ) {
                    set_error( "Division by zero." );
                    return;
                }
                set_field( get_field() / value.get_field() );
            }
            return;
        default:
           set_error( "Can only divide numbers." );
           return;
        }
    }
    set_error( "Must divide the same types." );
}

void SValue::rlist_union( const SValue& value )
{
    RangeList left, right;
    if( obtain_rlists( left, right, value ) ) {
        set_rlist( op_set_union( left, right ) );
    }
}

void SValue::intersection( const SValue& value )
{
    RangeList left, right;
    if( obtain_rlists( left, right, value ) ) {
        set_rlist( op_set_intersection( left, right ) );
    }
}

void SValue::rel_complement( const SValue& value )
{
    RangeList left, right;
    if( obtain_rlists( left, right, value ) ) {
        set_rlist( op_set_rel_complement( left, right ) );
    }
}

void SValue::sym_difference( const SValue& value )
{
    RangeList left, right;
    if( obtain_rlists( left, right, value ) ) {
        set_rlist( op_set_sym_difference( left, right ) );
    }
}

void SValue::range_op( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == SVT_Field || m_type == SVT_Range ) {
        Range range( get_field(), value.get_field() );
        set_range( range );
        return;
    }
    set_error( "Can only set range with numbers." );
}

void SValue::negate()
{
    if( is_error() ) {
        return;
    }
    switch( m_type )
    {
    case SVT_Range:
        m_range.jdn2 = m_range.jdn2;
        // fall thru
    case SVT_Field:
        m_range.jdn1 = -m_range.jdn1;
        return;
    }
    set_error( "Can only negate numbers." );
}

void SValue::logical_not()
{
    if( is_error() ) {
        return;
    }
    set_bool( !get_bool() );
}

void SValue::compliment()
{
    if( is_error() ) {
        return;
    }
    RangeList rlist;
    if( get_rlist( rlist ) ) {
        set_rlist( op_set_complement( rlist ) );
        return;
    }
    set_error( "Cannot convert to RList." );
}

Field SValue::add( Field left, Field right ) const
{
    // TODO: check for f_invalid etc and for overflow.
    return left + right;
}

Range SValue::add( Range range, Field field ) const
{
    return Range( add( range.jdn1, field ), add( range.jdn2, field ) );
}

Range SValue::add( Range left, Range right ) const
{
    return Range( 
        add( left.jdn1, right.jdn1 ),
        add( left.jdn2, right.jdn2 )
    );
}

RangeList SValue::add( RangeList rlist, Field field ) const
{
    RangeList rl;
    for( size_t i = 0 ; i < rlist.size() ; i++ ) {
        Range range = add( rlist[i], field );
        rl.push_back( range );
    }
    return rl;
}

RangeList SValue::add( RangeList rlist, Range range ) const
{
    RangeList rl;
    for( size_t i = 0 ; i < rlist.size() ; i++ ) {
        Range r = add( rlist[i], range );
        rl.push_back( r );
    }
    return rl;
}

// ----------------------------------------------

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

// ----------------------------------------------

Script::Script( Calendars* cals, istream& in, ostream& out ) 
    : m_cals(cals), m_ts(in,out), m_out(&out), m_err(&out)
{
    assert( cals != NULL );
}

bool Script::run()
{
    for(;;) {
        m_ts.next();
        if( m_ts.current().type() == SToken::STT_End ) {
            return true;
        }
        if( statement() == false ) {
            return m_ts.errors() == 0; 
        }
    }
    return false;
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
    SValue left = sum( get );
    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Equal:
            left.equal( sum( true ) );
            break;
        case SToken::STT_NotEqual:
            left.equal( sum( true ) );
            left.logical_not();
            break;
        case SToken::STT_GtThan:
            left.greater_than( sum( true ) );
            break;
        case SToken::STT_GtThanEq:
            left.less_than( sum( true ) );
            left.logical_not();
            break;
        case SToken::STT_LessThan:
            left.less_than( sum( true ) );
            break;
        case SToken::STT_LessThanEq:
            left.greater_than( sum( true ) );
            left.logical_not();
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
    SValue left = combine( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Star:
            left.multiply( combine( true ) );
            break;
        case SToken::STT_Divide:
            left.divide( combine( true ) );
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
    SValue left = primary( get );

    for(;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::STT_Tilde:
            left.range_op( primary( true ) );
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
    case SToken::STT_date:
        value = primary( true );
        {
            SHandle sch = store()->ischeme;
            if( sch ) {
                value.set( sch->rlist_str_to_rangelist( value.get_str() ) );
            } else {
                error( "Valid scheme not set." );
            }
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

SValue Script::get_value_var( const string& name )
{
    if( store()->table.count( name ) ) {
        return store()->table.find( name )->second;
    }
    SValue value;
    value.set_error( "Variable \"" + name + "\" not found." );
    return value;
}

// End of src/cal/calscript.cpp file
