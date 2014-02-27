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

#include "calparse.h"
#include "calscheme.h"
#include "calschemes.h"
#include "calscripts.h"
#include "calversion.h"
#include "calvocab.h"

#include <cassert>

using namespace std;
using namespace Cal;

Calendars::Calendars( Init_schemes init )
{
    m_schemes = new Schemes;
    switch( init )
    {
    case Init_schemes_none:
        break;
    case Init_schemes_default:
        read_script( cal_default_script );
            break;
    }
}

Calendars::~Calendars()
{
    delete m_schemes;
}

const char* Calendars::version()
{
    return cal_version;
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

void Calendars::get_scheme_input( Scheme_input* info, int scheme_id ) const
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->get_input( info );
}

void Calendars::get_scheme_output( Scheme_output* info, int scheme_id ) const
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->get_output( info );
}

void Calendars::get_vocab_info( Vocab_info* info, const string& code ) const
{
    Vocab* voc = m_schemes->get_vocab( code );
    voc->get_info( info );
}

void Calendars::set_scheme_order( int scheme_id, int order )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->set_order( order );
}

void Calendars::set_scheme_format( int scheme_id, int format )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    sch->set_format( format );
}

void Calendars::read_script( const string& script )
{
    vector<string> statements = parse_statements( script );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string body;
        string rule = get_first_word( statements[i], &body );
        if( rule == "scheme" ) {
            m_schemes->add_scheme( body );
        } else if( rule == "vocab" ) {
            m_schemes->add_vocab( body );
        } else if( rule == "grammar" ) {
            m_schemes->add_grammar( body );
        } else {
            assert( false );
        }
    }
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

Range Calendars::str_to_range( int scheme_id, const string& str/*, int i*/ )
{
    Scheme* sch = m_schemes->get_scheme( scheme_id );
    return sch->str_to_range( str/*, i*/ );
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

// End of src/cal/calcalendars.cpp file
