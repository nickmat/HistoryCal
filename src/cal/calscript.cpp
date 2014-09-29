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
#include "calregnal.h"
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
    SValue value = expr( true );
    string code;
    value.get( code );
    SHandle sch = do_create_scheme( code );
    return m_cals->add_scheme( sch, code );
}

SHandle Script::do_create_scheme( const std::string& code )
{
    if( m_ts.current().type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return NULL;
    }
    Base* base = NULL;
    string name;
    string gmr_code;
    Scheme_style style = SCH_STYLE_Default;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket || token.type() == SToken::STT_End ) {
            break;
        }
        if( token.type() == SToken::STT_Semicolon ) {
            continue;
        }
        if( token.type() == SToken::STT_Name ) {
            if( token.get_str() == "name" ) {
                expr( true ).get( name );
            } else if( token.get_str() == "base" ) {
                base = do_base();
            } else if( token.get_str() == "shift" ) {
                base = do_base_shift();
            } else if( token.get_str() == "hybrid" ) {
                base = do_base_hybrid();
            } else if( token.get_str() == "regnal" ) {
                base = do_base_regnal();
            } else if( token.get_str() == "grammar" ) {
                expr( true ).get( gmr_code );
            } else if( token.get_str() == "style" ) {
                token = m_ts.next();
                if( token.type() != SToken::STT_Name ) {
                    error( "Style name expected." );
                    continue;
                }
                if( token.get_str() == "hide" ) {
                    style = SCH_STYLE_Hide;
                }
            }
        }
    }
    if( base == NULL ) {
        return NULL;
    }
    SHandle sch = new Scheme( name, base );
    sch->set_style( style );
    sch->set_grammar( m_cals->get_grammar( gmr_code ) );
    sch->set_code( code );
    return sch;
}

Base* Script::do_base()
{
    Scheme::BaseScheme bs = Scheme::BS_NULL;
    SToken token = m_ts.next();
    if( token.type() == SToken::STT_Name ) {
        if( token.get_str() == "jdn" ) {
            bs = Scheme::BS_jdn;
        } else if( token.get_str() == "julian" ) {
            bs = Scheme::BS_julian;
        } else if( token.get_str() == "gregorian" ) {
            bs = Scheme::BS_gregorian;
        }
    } else {
        error( "Base name expected." );
    }
    return Scheme::create_base( bs );
}

Base* Script::do_base_shift()
{
    string scode;
    expr( true ).get( scode );
    SHandle sch = m_cals->get_scheme( scode );
    Field era = f_invalid;
    if( sch && m_ts.current().type() == SToken::STT_Comma ) {
        era = expr( true ).get_field();
    }
    Base* sbase = NULL;
    if( sch ) {
        sbase = sch->get_base();
    }
    return Scheme::create_base_shift( sbase, era );
}

Base* Script::do_base_hybrid()
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return NULL;
    }
    StringVec fieldnames;
    vector<Base*> bases;
    FieldVec dates;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket || token.type() == SToken::STT_End ) {
            break;
        }
        if( token.type() == SToken::STT_Semicolon ) {
            continue;
        }
        SValue value;
        if( token.type() == SToken::STT_Name ) {
            if( token.get_str() == "fields" ) {
                fieldnames = do_string_list();
            } else if( token.get_str() == "schemes" ) {
                do_base_date_list( bases, dates );
            }
        }
    }
    return Scheme::create_base_hybrid( fieldnames, bases, dates );
}

Base* Script::do_base_regnal()
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return NULL;
    }
    StringVec fieldnames;
    vector<RegnalEra> eras;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket || token.type() == SToken::STT_End ) {
            break;
        }
        if( token.type() == SToken::STT_Semicolon ) {
            continue;
        }
        SValue value;
        if( token.type() == SToken::STT_Name ) {
            if( token.get_str() == "fields" ) {
                fieldnames = do_string_list();
            } else if( token.get_str() == "era" ) {
                RegnalEra era;
                if( do_regnal_era( era, fieldnames ) ) {
                    eras.push_back( era );
                }
            }
        }
    }
    return Scheme::create_base_regnal( fieldnames, eras );
}

bool Script::do_regnal_era( RegnalEra& era, const StringVec& fieldnames )
{
    SToken token = m_ts.next();
    if( token.type() != SToken::STT_LCbracket ) {
        error( "'{' expected." );
        return NULL;
    }
    Range range( f_minimum, f_maximum );
    StringVec matchs;
    SHandle sch;
    bool local = true;
    for(;;) {
        SToken token = m_ts.next();
        if( token.type() == SToken::STT_RCbracket || token.type() == SToken::STT_End ) {
            break;
        }
        if( token.type() == SToken::STT_Semicolon ) {
            continue;
        }
        SValue value;
        if( token.type() == SToken::STT_Name ) {
            if( token.get_str() == "range" ) {
                SValue value = expr( true );
                range = value.get_range();
            } else if( token.get_str() == "scheme" ) {
                SValue value = expr( true );
                string scode;
                value.get( scode );
                if( m_ts.current().type() == SToken::STT_Semicolon ) {
                    sch = m_cals->get_scheme( scode );
                    local = false;
                } else {
                    // Create new scheme 
                    sch = do_create_scheme( scode );
                }
            }
        } else if( token.type() == SToken::STT_match ) { // "match" is a keyword
            matchs = do_string_list();
            assert( matchs.size() % 2 == 0 ); // must be even number
        }
    }
    era.begin = range.jdn1;
    era.end = range.jdn2;
    era.base = sch->get_base();
    era.scheme = sch;
    era.local = local;
    era.xref.clear();
    for( size_t i = 0 ; i < fieldnames.size() ; i++ ) {
        string fieldname = fieldnames[i];
        for( size_t j = 0 ; j < matchs.size() ; j += 2 ) {
            if( fieldname == matchs[j+1] ) {
                fieldname = matchs[j];
                break;
            }
        }
        int index = era.base->get_fieldname_index( fieldname );
        era.xref.push_back( index );
    }
    return true;
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

StringVec Script::do_string_list()
{
    StringVec vec;
    for(;;) {
        SValue value = expr( true );
        string str;
        if( ! value.get( str ) || str.empty() ) {
            break;
        }
        vec.push_back( str );
        if( m_ts.current().type() != SToken::STT_Comma ) {
            break;
        }
    }
    return vec;
}

bool Script::do_base_date_list( vector<Base*>& bases, FieldVec& dates )
{
    for(;;) {
        SValue value = expr( true );
        string scode;
        if( ! value.get( scode ) || scode.empty() ) {
            return false;
        }
        Base* base = m_cals->get_scheme( scode )->get_base();
        if( base == NULL ) {
            error( "Invalid scheme code." );
            return false;
        }
        bases.push_back( base );
        if( m_ts.current().type() != SToken::STT_Comma ) {
            return m_ts.current().type() == SToken::STT_Semicolon;
        }
        value = expr( true );
        Field date = value.get_field();
        if( date == f_invalid ) {
            error( "Invalid date." );
            return false;
        }
        dates.push_back( date );
    }
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

string Script::get_name_or_string( const SToken& token ) const
{
    string str;
    if( token.type() == SToken::STT_Name || token.type() == SToken::STT_String ) {
        str = token.get_str();
    }
    return str;
}

// End of src/cal/calscript.cpp file
