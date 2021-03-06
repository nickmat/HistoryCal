/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscheme.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Scheme class implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
 * Copyright:   Copyright (c) 2013 .. 2021, Nick Matthews.
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

#include "calscheme.h"

#include "cal/calendars.h"
#include "calbase.h"
#include "calchinese.h"
#include "caleaster.h"
#include "calformat.h"
#include "calfrench.h"
#include "calgrammar.h"
#include "calgregorian.h"
#include "calhebrew.h"
#include "calhybrid.h"
#include "calislamic.h"
#include "calisoordinal.h"
#include "calisoweek.h"
#include "caljdn.h"
#include "caljulian.h"
#include "calmath.h"
#include "calparse.h"
#include "calrecord.h"
#include "calregnal.h"
#include "calshift.h"
#include "calshiftday.h"

#include <cassert>
#include <cstdlib>

using namespace Cal;
using namespace std;


Scheme::Scheme( const std::string& name, const Base* base ) 
    : m_name(name), m_style(SCH_STYLE_Default),
    m_owns_base( true ), m_base(base)
{
}

Scheme::~Scheme()
{
    if ( m_owns_base ) {
        delete m_base;
    }
}

bool Scheme::is_ok() const
{
    return m_base && m_code.size();
}

string Scheme::get_pref_input_format() const
{
    Grammar* gmr = m_base->get_grammar();
    if( gmr ) {
        return gmr->get_pref_input_fcode();
    }
    return "def";
}

void Scheme::get_info( Scheme_info* info ) const
{
    info->name = m_name;
    info->code = m_code;
    info->style = m_style;
    Grammar* gmr = m_base->get_grammar();
    if( gmr ) {
        info->grammar_code = gmr->code();
        info->vocab_codes = gmr->get_vocab_codes();
        info->vocab_names = gmr->get_vocab_names();
    } else {
        info->grammar_code = "";
    }
}

string Scheme::get_input_fcode() const
{
    return m_input_fcode.empty() ? "def" : m_input_fcode;
}

string Scheme::get_output_fcode() const
{
    return m_output_fcode.empty() ? "def" : m_output_fcode;
}

Format* Scheme::get_output_format( const string& fcode ) const
{
    if( fcode.empty() ) {
        return get_base()->get_format( m_output_fcode );
    }
    return get_base()->get_format( fcode );
}

Field Scheme::fieldvec_to_jdn( const FieldVec& fieldv )
{
    if( fieldv.size() < m_base->record_size() ) {
        return f_invalid;
    }
    Record rec( m_base, &fieldv[0], m_base->record_size() );
    return rec.get_jdn();
}

FieldVec Scheme::jdn_to_fieldvec( Field jdn )
{
    Record rec( m_base, jdn );
    return rec.get_fieldvec();
}

RangeList Scheme::fieldvec_to_rlist( const FieldVec& fieldv )
{
    Record rec( m_base, &fieldv[0], fieldv.size() ); 
    return rec.get_rlist_from_mask();
}

Field Scheme::jdn_fieldname_to_field( Field jdn, const string& fieldname ) const
{
    Record rec( m_base, jdn );
    return rec.get_field( fieldname );
}

Field Scheme::str_to_jdn( const string& str, const string& fmt )
{
    Record rec( m_base, str, fmt, RB_none );
    return rec.get_jdn();
}

string Scheme::jdn_to_str( Field jdn, const string& fcode )
{
    Format* fmt;
    if ( fcode.empty() ) {
        fmt = get_output_format( get_output_fcode() );
    } else {
        fmt = get_output_format( fcode );
    }
    if ( fmt == nullptr ) {
        return "(" + fcode + ") Format not found.";
    }
    return fmt->jdn_to_string( get_base(), jdn );
}

string Scheme::range_to_str( Range range, const string& fcode )
{
    Format* fmt;
    if ( fcode.empty() ) {
        fmt = get_output_format( get_output_fcode() );
    } else {
        fmt = get_output_format( fcode );
    }
    if ( fmt == nullptr ) {
        return "(" + fcode + ") Format not found.";
    }
    return fmt->range_to_string( get_base(), range );
}

string Scheme::rangelist_to_str( const RangeList& ranges, const std::string& fcode )
{
    if( ranges.empty() ) {
        return "";
    }
    Format* fmt;
    if ( fcode.empty() ) {
        fmt = get_output_format( get_output_fcode() );
    } else {
        fmt = get_output_format( fcode );
    }
    if ( fmt == nullptr ) {
        return "(" + fcode + ") Format not found.";
    }
    return fmt->rlist_to_string( get_base(), ranges );
}

Field Scheme::add_to_jdn( Field jdn, Field value, Unit unit, Norm norm )
{
    if( unit == unit_day ) {
        return jdn + value;
    }
    Record rec( m_base, jdn );
    Field days = rec.is_unit_int( unit );
    if( days ) {
        return jdn + value * days;
    }
    if( norm != norm_average && rec.can_add_unit( unit ) ) {
        if( rec.add( value, unit, norm ) ) {
            return rec.get_jdn();
        }
    }
    double ddays = rec.get_average_days( unit );
    if( ddays > 0.0 ) {
        return jdn + round_f( ddays * (double) value );
    }
    assert( false ); // Run out of ideas!
    return jdn;
}

bool Scheme::str_to_rel_info( const string& str, Rel_info* info ) const
{
    string tail;
    string word = get_first_word( str, &tail );
    if( word == "age" ) {
        info->rel = rel_age_round_down;
    } else {
        // Don't know how to handle anything else
        return false;
    }
    word = get_first_word( tail, &tail );
    info->value = strtol( word.c_str(), NULL, 10 );
    info->unit = unit_year;
    if( tail.size() ) {
        info->unit = str_to_unit( tail );
    }
    info->norm = norm_expand_min;
    if( info->value <= 0 || info->unit == unit_null ) {
        // Must have positive value and valid unit
        return false;
    }
    return true;
}

RangeList Scheme::rel_rangelist( const RangeList& ranges, Rel_info* info )
{
    RangeList rs;

    for( size_t i = 0 ; i < ranges.size() ; i++ ) {
        Range r;
        switch( info->rel )
        {
        case rel_age_round_down:
            r.jdn1 = add_to_jdn( ranges[i].jdn1, -(info->value+1), info->unit, info->norm )+1;
            r.jdn2 = add_to_jdn( ranges[i].jdn2, -info->value, info->unit, info->norm );
            break;
        default:
            continue;
        }
        rs.push_back( r );
    }
    return rs;
}

Unit Scheme::str_to_unit( const std::string& str ) const
{
    Unit unit = unit_null;
    Grammar* gmr = m_base->get_grammar();
    if( gmr ) {
        unit = gmr->get_unit_alias( str );
    }
    if( unit == unit_null ) {
        if( str == "year" ) {
            unit = unit_year;
        } else if( str == "month" ) {
            unit = unit_month;
        } else if( str == "week" ) {
            unit = unit_week;
        } else if( str == "day" ) {
            unit = unit_day;
        }
    }
    return unit;
}

Base* Scheme::create_base( BaseName bs, const std::string& data ) 
{
    switch( bs )
    {
    case BaseName::jdn:
        return new Jdn( data );
    case BaseName::julian:
        return new Julian( data );
    case BaseName::gregorian:
        return new Gregorian( data );
    case BaseName::isoweek:
        return new IsoWeek( data );
    case BaseName::isoordinal:
        return new IsoOrdinal( data );
    case BaseName::french:
        return new French( data );
    case BaseName::hebrew:
        return new Hebrew( data );
    case BaseName::islamic:
        return new Islamic( data );
    case BaseName::chinese:
        return new Chinese( data );
    case BaseName::easter:
        return new Easter( data );
    default:
        return nullptr;
    }
}

Base* Scheme::create_base_shift( const Base* sbase, Field epoch )
{
    assert( sbase != nullptr );
    Base* base;
    if( sbase->record_size() == 1 ) {
        base = new ShiftDay( sbase, epoch );
    } else {
        base = new Shift( sbase, epoch );
    }
    if( base->is_ok() ) {
        return base;
    }
    delete base;
    return nullptr;
}

Base* Scheme::create_base_hybrid( 
    const StringVec& fieldnames,
    const vector<HybridData>& data )
{
    Base* base = new Hybrid( fieldnames, data );
    if( base->is_ok() ) {
        return base;
    }
    delete base;
    return NULL;
}

Base* Scheme::create_base_regnal( 
    const StringVec& fieldnames,
    const StringVec& ext_fieldnames,
    const FieldVec& fixedfields,
    const vector<RegnalEra>& eras )
{
    Base* base = new Regnal( fieldnames, ext_fieldnames, fixedfields, eras );
    if( base->is_ok() ) {
        return base;
    }
    delete base;
    return NULL;
}


// End of src/cal/calscheme.cpp file
