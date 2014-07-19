/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calcalendars.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Main library API.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     19th September 2013
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

#include "cal/calendars.h"

#include "caldefscripts.h"
#include "calgrammar.h"
#include "calparse.h"
#include "calrecord.h"
#include "calscheme.h"
#include "calscript.h"
#include "calscriptstore.h"
#include "calversion.h"
#include "calvocab.h"

#include <cassert>
#include <sstream>

using namespace std;
using namespace Cal;


Calendars::Calendars( Init_schemes init )
{
    m_store = new ScriptStore;
    switch( init )
    {
    case Init_script_none:
        break;
    case Init_script_default:
        run_script( cal_default_script );
        break;
    }
}

Calendars::~Calendars()
{
    for( SHandleMap::iterator it = m_shandles.begin() ; it != m_shandles.end() ; it++ ) {
        delete it->second;
    }
    for( GrammarMap::iterator it = m_grammars.begin() ; it != m_grammars.end() ; it++ ) {
        delete it->second;
    }
    for( VocabMap::iterator it = m_vocabs.begin() ; it != m_vocabs.end() ; it++ ) {
        delete it->second;
    }
    delete m_store;
}

const char* Calendars::version()
{
    return cal_version;
}

string Calendars::run_script( const string& script )
{
    istringstream iss( script );
    ostringstream oss;
    Script scr( this, iss, oss );
    scr.run();
    return oss.str();
}

SHandle Calendars::get_scheme( const string& code ) const
{
    if( m_shandles.count( code ) > 0 ) {
        return m_shandles.find( code )->second;
    }
    return NULL;
}

SchemeList Calendars::get_scheme_list() const
{
    SchemeList slist;
    SchemeData sdata;
    SHandleMap::const_iterator it;
    for( it = m_shandles.begin() ; it != m_shandles.end() ; it++ ) {
        if( it->second->get_style() == SCH_STYLE_Hide ) {
            continue;
        }
        sdata.code = it->first;
        sdata.handle = it->second;
        sdata.name = sdata.handle->get_name();
        slist.push_back( sdata );
    }
    return slist;
}

void Calendars::get_scheme_info( Scheme_info* info, SHandle scheme ) const
{
    scheme->get_info( info );
}

void Calendars::get_scheme_input( SchemeFormats* info, SHandle scheme ) const
{
    scheme->get_input( info );
}

void Calendars::get_scheme_output( SchemeFormats* info, SHandle scheme ) const
{
    scheme->get_output( info );
}

bool Calendars::get_vocab_info( Vocab_info* info, const string& code ) const
{
    Vocab* voc = get_vocab( code );
    if( voc == NULL ) {
        return false;
    }
    voc->get_info( info );
    return true;
}

void Calendars::set_input_format( SHandle scheme, const std::string& code )
{
    scheme->set_input_format( code );
}

void Calendars::set_output_format( SHandle scheme, const std::string& code )
{
    scheme->set_output_format( code );
}

Field Calendars::fieldvec_to_jdn( SHandle scheme, const FieldVec& fieldv )
{
    return scheme->fieldvec_to_jdn( fieldv );
}

FieldVec Calendars::jdn_to_fieldvec( SHandle scheme, Field jdn )
{
    return scheme->jdn_to_fieldvec( jdn );
}

Field Calendars::str_to_jdn( SHandle scheme, const string& str )
{
    return scheme->str_to_jdn( str );
}

string Calendars::jdn_to_str( SHandle scheme, Field jdn )
{
    return scheme->jdn_to_str( jdn );
}

Range Calendars::str_to_range( SHandle scheme, const string& str )
{
    return scheme->str_to_range( str );
}

string Calendars::range_to_str( SHandle scheme, Range range )
{
    return scheme->range_to_str( range );
}

RangeList Calendars::str_to_rangelist( SHandle scheme, const string& input )
{
//    return scheme->rlist_str_to_rangelist( str );

    RangeList rlist;
    string str;
    size_t pos, cpos = 0;
    do {
        pos = input.find( range_div, cpos );
        if( pos != string::npos ) {
            str = input.substr( cpos, pos - cpos );
            cpos = pos + 1;
        } else {
            str = input.substr( cpos );
        }
        RangeList rl = scheme->r_str_to_rangelist( str );
        if( rl.size() ) {
            rlist.insert( rlist.end(), rl.begin(), rl.end() );
        }
    } while( pos != string::npos );
    return rlist;

}

RangeList Calendars::expr_str_to_rangelist( SHandle scheme, const string& str )
{
    RangeList rlist;
    if( scheme == NULL ) {
        return rlist;
    }
    string script = "set input \"" + scheme->get_code() + "\";\n"
        + "let result = " + parse_date_expr( str ) + ";";

    istringstream iss( script );
    ostringstream oss;
    Script scr( this, iss, oss );
    scr.run();

    SValueMap table = get_store()->table;
    if( table.count( "result" ) > 0 ) {
        table.find( "result" )->second.get_rlist( rlist );
    }
    return rlist;
}

string Calendars::rangelist_to_str( SHandle scheme, const RangeList& ranges )
{
    return scheme->rangelist_to_str( ranges );
}

Field Calendars::add_to_jdn( SHandle scheme, Field jdn, Field value, Unit unit, Norm norm )
{
    return scheme->add_to_jdn( jdn, value, unit, norm );
}

bool Calendars::str_to_rel_info( SHandle scheme, const string& str, Rel_info* info ) const
{
    return scheme->str_to_rel_info( str, info );
}

RangeList Calendars::rel_rangelist( SHandle scheme, const RangeList& ranges, Rel_info* info )
{
    return scheme->rel_rangelist( ranges, info );
}

Grammar* Calendars::get_grammar( const string& code ) const
{
    if( m_grammars.count( code ) > 0 ) {
        return m_grammars.find( code )->second;
    }
    return NULL;
}

Vocab* Calendars::get_vocab( const string& code ) const
{
    if( m_vocabs.count( code ) > 0 ) {
        return m_vocabs.find( code )->second;
    }
    return NULL;
}

void Calendars::add_scheme( const string& definition )
{
    SHandle sch = new Scheme( this, definition );
    if( sch->is_ok() == false ) {
        delete sch;
        return;
    }
    string code = sch->get_code();
    m_shandles[code] = sch;
}

void Calendars::add_vocab( const string& definition )
{
    Vocab* voc = new Vocab( definition );
    string code = voc->get_code();
    m_vocabs[code] = voc;
}

Grammar* Calendars::add_grammar( const string& code )
{
    Grammar* gmr = new Grammar( code );
    m_grammars[code] = gmr;
    return gmr;
}

// Convert a string written as a shorthand or single longhand range
// to a rangelist.
RangeList Calendars::r_str_to_rangelist( SHandle scheme, const string& str )
{
    string str1 = str, str2;
    Base* base1 = scheme->get_base();
    Base* base2 = base1;
    size_t pos = str.find( range_sep );
    if( pos == string::npos ) {
        // Short form range string
        pos = str.find( '#' );
        // Check  for scheme prefix
        if( pos != string::npos ) {
            string scode = str.substr( 0, pos );
            SHandle sch = get_scheme( scode );
            base2 = base1 = sch->get_base();
            str1 = str.substr( pos + 1 );
        }
        str2 = str1;
    } else {
        // Two date strings
        string str1 = str.substr( 0, pos );
        string str2 = str.substr( pos + 1 );
        // Check range start date for scheme prefix
        pos = str1.find( '#' );
        if( pos != string::npos ) {
            string scode = str1.substr( 0, pos );
            SHandle sch = get_scheme( scode );
            base2 = base1 = sch->get_base();
            str1 = str1.substr( pos + 1 );
        }
        // Check range finish date for scheme prefix
        pos = str2.find( '#' );
        if( pos != string::npos ) {
            string scode = str2.substr( 0, pos );
            SHandle sch = get_scheme( scode );
            base2 = sch->get_base();
            str2 = str2.substr( pos + 1 );
        }
    }
    str1 = full_trim( str1 );
    str2 = full_trim( str2 );

    RangeList ranges;
    Record mask1(base1, str1), mask2(base2, str2);
    Record rec1(base1), rec2(base2);
    bool ret1 = rec1.set_fields_as_begin_first( mask1.get_field_ptr() );
    bool ret2 = rec2.set_fields_as_begin_last( mask2.get_field_ptr() );
    while( ret1 && ret2 ) {
        Range range;
        range.jdn1 = rec1.get_jdn();
        range.jdn2 = rec2.get_jdn();
        if( range.jdn1 > range.jdn2 ) {
            break;
        }
        ranges.push_back( range );
        ret1 = rec1.set_fields_as_next_first( mask1.get_field_ptr() );
        ret2 = rec2.set_fields_as_next_last( mask2.get_field_ptr() );
    }
    return ranges;


#if 0
    Range range;
    bool ret1 = false, ret2 = false;
    Record mask1(m_base), mask2(m_base);
    Record rec1(m_base), rec2(m_base);
    size_t pos = str.find( range_sep );
    if( pos == string::npos ) {
        // Short form range string
        mask1.set_str( str );
        mask2.set_fields( mask1.get_field_ptr(), m_base->extended_size() );
    } else {
        // two date strings
        string str1 = str.substr( 0, pos );
        string str2 = str.substr( pos + 1 );
        pos = str1.find( "past" );
        if( pos != string::npos ) {
            range.jdn1 = f_minimum;
            ret1 = true;
        } else {
            mask1.set_str( str1 );
        }
        pos = str2.find( "future" );
        if( pos != string::npos ) {
            range.jdn2 = f_maximum;
            ret2 = true;
        } else {
            mask2.set_str( str2 );
        }
        if( ret1 || ret2 ) {
            if( ret1 == false ) {
                rec1.set_fields_as_begin_first( mask1.get_field_ptr() );
                range.jdn1 = rec1.get_jdn();
            }
            if( ret2 == false ) {
                rec2.set_fields_as_begin_last( mask2.get_field_ptr() );
                range.jdn2 = rec2.get_jdn();
            }
            ranges.push_back( range );
            return ranges;
        }
    }
    ret1 = rec1.set_fields_as_begin_first( mask1.get_field_ptr() );
    ret2 = rec2.set_fields_as_begin_last( mask2.get_field_ptr() );
    while( ret1 && ret2 ) {
        range.jdn1 = rec1.get_jdn();
        range.jdn2 = rec2.get_jdn();
        if( range.jdn1 > range.jdn2 ) {
            break;
        }
        ranges.push_back( range );
        ret1 = rec1.set_fields_as_next_first( mask1.get_field_ptr() );
        ret2 = rec2.set_fields_as_next_last( mask2.get_field_ptr() );
    }
    return ranges;
#endif
}


// End of src/cal/calcalendars.cpp file
