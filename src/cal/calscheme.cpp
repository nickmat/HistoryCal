/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calscheme.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Scheme class implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     20th September 2013
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

#include "calscheme.h"

#include "calbase.h"
#include "cal/calendars.h"
#include "calgrammar.h"
#include "calgregorian.h"
#include "calhybrid.h"
#include "caljdn.h"
#include "caljulian.h"
#include "calmath.h"
#include "calparse.h"
#include "calrecord.h"
#include "calregnal.h"
#include "calshift.h"

#include <cassert>

using namespace Cal;
using namespace std;

Scheme::Scheme( Calendars* cals, const string& definition ) 
    : m_style(SCH_STYLE_Default), m_base(NULL)
{
    string body;
    m_code = get_first_word( definition, &body );
    vector<string> statements = parse_statements( peel_cbrackets( body ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string statement = get_first_word( statements[i], &body );
        if( statement == "name" ) {
            m_name = body;
        } else if( statement == "base" ) {
            if( body == "jdn" ) {
                m_base = new Jdn;
            } else if( body == "julian" ) {
                m_base = new Julian;
            } else if( body == "gregorian" ) {
                m_base = new Gregorian;
            } else {
                assert( false ); // Shouldn't be here
            }
        } else if( statement == "shift" ) {
            m_base = new Shift( cals, body );
        } else if( statement == "hybrid" ) {
            m_base = new Hybrid( cals, body );
        } else if( statement == "regnal" ) {
            m_base = new Regnal( cals, m_code, body );
        } else if( statement == "grammar" ) {
            if( m_base ) {
                Grammar* gmr = cals->get_grammar( body );
                m_base->set_grammar( gmr );
            }
        } else if( statement == "style" ) {
            if( body == "hide" ) {
                m_style = SCH_STYLE_Hide;
            }
        }
    }
}

Scheme::~Scheme()
{
    delete m_base;
}

bool Scheme::is_ok() const
{
    return m_base && m_code.size() && m_name.size();
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

void Scheme::get_input( SchemeFormats* info ) const
{
    m_base->get_input_formats( info );
}

void Scheme::get_output( SchemeFormats* info ) const
{
    m_base->get_output_formats( info );
}

void Scheme::set_grammar( Grammar* grammar )
{
    m_base->set_grammar( grammar );
}

void Scheme::set_input_format( const std::string& code )
{
    m_base->set_input_format( code );
}

void Scheme::set_output_format( const std::string& code )
{
    m_base->set_output_format( code );
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

Field Scheme::str_to_jdn( const string& str )
{
    Record rec( m_base, str );
    return rec.get_jdn();
}

string Scheme::jdn_to_str( Field jdn )
{
    if( jdn == f_minimum ) {
        return "past.";
    }
    if( jdn == f_maximum ) {
        return "future.";
    }
    Record rec( m_base, jdn );
    return rec.get_str();
}

Range Scheme::str_to_range( const string& str )
{
    RangeList rlist = r_str_to_rangelist( str );
    switch( rlist.size() )
    {
    case 0:
        return Range( f_invalid, f_invalid );
    case 1:
        return rlist[0];
    }
    return Range( rlist[0].jdn1, rlist[rlist.size()-1].jdn2 );
}

string Scheme::range_to_str( Range range )
{
    if( range.jdn1 == range.jdn2 ) {
        return jdn_to_str( range.jdn1 );
    }
    string str1, str2;
    if( range.jdn1 == f_minimum || range.jdn2 == f_maximum ) {
        str1 = jdn_to_str( range.jdn1 );
        str2 = jdn_to_str( range.jdn2 );
    } else {
        Record rec1( m_base, range.jdn1 );
        Record rec2( m_base, range.jdn2 );

        rec1.remove_balanced_fields( &rec2 );
        str1 = rec1.get_str();
        str2 = rec2.get_str();
        if( str1 == str2 ) {
            return str1;
        }
    }
    return str1 + " ~ " + str2;
}

// Convert a string which contains a list of ranges to a rangelist.
RangeList Scheme::rlist_str_to_rangelist( const string& input )
{
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
        RangeList rl = r_str_to_rangelist( str );
        if( rl.size() ) {
            rlist.insert( rlist.end(), rl.begin(), rl.end() );
        }
    } while( pos != string::npos );
    return rlist;
}

// Convert a string written as a shorthand or single longhand range
// to a rangelist.
RangeList Scheme::r_str_to_rangelist( const string& str )
{
    RangeList ranges;
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
        pos = str1.find( "past." );
        if( pos != string::npos ) {
            range.jdn1 = f_minimum;
            ret1 = true;
        } else {
            mask1.set_str( str1 );
        }
        pos = str2.find( "future." );
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
}

string Scheme::rangelist_to_str( const RangeList& ranges )
{
    string str;
    for( size_t i = 0 ; i < ranges.size() ; i++ ) {
        if( i > 0 ) {
            str += " | ";
        }
        str += range_to_str( ranges[i] );
    }
    return str;
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
        return jdn + cal_round( ddays * (double) value );
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

// End of src/cal/calscheme.cpp file
