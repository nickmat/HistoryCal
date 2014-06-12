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

#include "calgrammar.h"
#include "calparse.h"
#include "calscheme.h"
#include "calschemes.h"
#include "calscript.h"
#include "caldefscripts.h"
#include "calversion.h"
#include "calvocab.h"

#include <cassert>

using namespace std;
using namespace Cal;


Calendars::Calendars( Init_schemes init )
{
    m_schemes = new Schemes;

    m_store = new ScriptStore;
    switch( init )
    {
    case Init_script_none:
        break;
    case Init_script_default:
        run_script( cal_default_script );
            break;
    case Init_schemes_default:
        read_script( cal_default_script );
            break;
    }
}

Calendars::~Calendars()
{
    delete m_schemes;
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
    Script scr( this );
    if( scr.run( script ) ) {
        return scr.get_output();
    }
    return "";
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
        voc = m_schemes->get_vocab( code );
    }
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

RangeList Calendars::str_to_rangelist( SHandle scheme, const string& str )
{
    return scheme->rlist_str_to_rangelist( str );
}

RangeList Calendars::expr_str_to_rangelist( SHandle scheme, const string& str )
{
    RangeList rlist;
    if( scheme == NULL ) {
        return rlist;
    }
    string script = "set input \"" + scheme->get_code() + "\";\n"
        + "evaluate date " + parse_date_expr( str ) + ";";

    Script scr( this );
    if( scr.run( script ) ) {
        return scr.get_date();
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

//#############################################################

string Calendars::read_script( const string& script )
{
    return m_schemes->read_script( script );
}

int Calendars::get_scheme_count() const
{
    return m_schemes->get_scheme_count();
}

int Calendars::get_scheme_id( const string& code ) const
{
    return m_schemes->get_scheme_id( code );
}

void Calendars::get_scheme_info( Scheme_info* info, int scheme_id ) const
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->get_info( info );
}

void Calendars::get_scheme_input( SchemeFormats* info, int scheme_id ) const
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->get_input( info );
}

void Calendars::get_scheme_output( SchemeFormats* info, int scheme_id ) const
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->get_output( info );
}

void Calendars::set_input_format( int scheme_id, const std::string& code )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->set_input_format( code );
}

void Calendars::set_output_format( int scheme_id, const std::string& code )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->set_output_format( code );
}

Field Calendars::fieldvec_to_jdn( int scheme_id, const FieldVec& fieldv )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->fieldvec_to_jdn( fieldv );
}

FieldVec Calendars::jdn_to_fieldvec( int scheme_id, Field jdn )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->jdn_to_fieldvec( jdn );
}

Field Calendars::str_to_jdn( int scheme_id, const string& str )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->str_to_jdn( str );
}

string Calendars::jdn_to_str( int scheme_id, Field jdn )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->jdn_to_str( jdn );
}

Range Calendars::str_to_range( int scheme_id, const string& str )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->str_to_range( str );
}

string Calendars::range_to_str( int scheme_id, Range range )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->range_to_str( range );
}

RangeList Calendars::str_to_rangelist( int scheme_id, const string& str )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->rlist_str_to_rangelist( str );
}

RangeList Calendars::expr_str_to_rangelist( int scheme_id, const string& str )
{
    return m_schemes->expr_str_to_rangelist( scheme_id, str );
}

string Calendars::rangelist_to_str( int scheme_id, const RangeList& ranges )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->rangelist_to_str( ranges );
}

Field Calendars::add_to_jdn( int scheme_id, Field jdn, Field value, Unit unit, Norm norm )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->add_to_jdn( jdn, value, unit, norm );
}

bool Calendars::str_to_rel_info( int scheme_id, const string& str, Rel_info* info ) const
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->str_to_rel_info( str, info );
}

RangeList Calendars::rel_rangelist( int scheme_id, const RangeList& ranges, Rel_info* info )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->rel_rangelist( ranges, info );
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

// End of src/cal/calcalendars.cpp file
