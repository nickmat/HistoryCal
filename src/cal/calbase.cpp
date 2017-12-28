/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calbase.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base class, used as base for all calendar calulations.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
 * Copyright:   Copyright (c) 2013 ~ 2017, Nick Matthews.
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

#include "calbase.h"

#include "calastro.h"
#include "calformattext.h"
#include "calformatunit.h"
#include "calgrammar.h"
#include "calmath.h"
#include "calparse.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;

const char* Base::s_ymd_fieldnames[] = { "year", "month", "day" };
size_t Base::s_sizeof_ymd_fieldnames = sizeof(s_ymd_fieldnames) / sizeof(const char*);

Base::Base() : m_grammar(NULL)
{
}

Base::Base( const std::string& data ) : m_grammar(NULL)
{
    string tail, word = get_first_word( data, &tail );
    while ( !word.empty() ) {
        set_data( word );
        word = get_first_word( tail, &tail );
    }
}

Base::~Base()
{
    // Default grammar has no code
    if( m_grammar && m_grammar->code() == "" ) {
        delete m_grammar;
    }
}

void Cal::Base::set_data( const std::string & data )
{
    string code, tail;
    split_code( &code, &tail, data );
    if ( code == "loc" ) {
        split_code( &code, &tail, tail );
        m_locale.lat = str_to_double( code );
        split_code( &code, &tail, tail );
        m_locale.lon = str_to_double( code );
    }
}

int Base::get_fieldname_index( const string& fieldname ) const
{
    int index = get_std_fieldname_index( fieldname );
    if( index < 0 ) {
        for( size_t i = 0 ; i < m_opt_fields.size() ; i++ ) {
            if( get_opt_fieldname( m_opt_fields[i] ) == fieldname ) {
                return record_size() + i;
            }
        }
    }
    return index;
}

string Base::get_fieldname( size_t index ) const
{
    if( index < record_size() ) {
        return get_std_fieldname( index );
    }
    if( index < extended_size() ) {
        return get_opt_fieldname( opt_index_to_id( index ) );
    }
    return "";
}

OptFieldID Base::get_opt_field_id( const std::string& fieldname ) const
{
    if( fieldname == "wday" ) {
        return OFID_wday;
    } 
    if( fieldname == "wsday" ) {
        return OFID_wsday;
    }
    if( fieldname == "dayinyear" ) {
        return OFID_dayinyear;
    }
    if( fieldname == "unshift" ) {
        return OFID_unshift;
    }
    if( fieldname == "nequinox" ) {
        return OFID_nequinox;
    }
    if( fieldname == "nsolstice" ) {
        return OFID_nsolstice;
    }
    if( fieldname == "sequinox" ) {
        return OFID_sequinox;
    }
    if( fieldname == "ssolstice" ) {
        return OFID_ssolstice;
    }
    if( fieldname == "newmoon" ) {
        return OFID_newmoon;
    }
    return OFID_NULL;
}

std::string Base::get_opt_fieldname( OptFieldID field_id ) const
{
    switch( field_id )
    {
    case OFID_wday:
        return "wday";
    case OFID_wsday:
        return "wsday";
    case OFID_dayinyear:
        return "dayinyear";
    case OFID_unshift:
        return "unshift";
    case OFID_nequinox:
        return "nequinox";
    case OFID_nsolstice:
        return "nsolstice";
    case OFID_sequinox:
        return "sequinox";
    case OFID_ssolstice:
        return "ssolstice";
    case OFID_newmoon:
        return "newmoon";
    default:
        return "";
    }
}

Field Base::get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const
{
    switch( id )
    {
    case OFID_wday:
        return day_of_week( jdn ) + 1; // Mon=1, Sun=7
    case OFID_wsday:
        return day_of_week( jdn + 1 ) + 1; // Sun=1, Sun=7
    case OFID_nequinox: // Northward/Spring equinox.
        return int( solar_longitude_after( spring, jdn ) + m_locale.lon / 360 );
    case OFID_nsolstice: // Northern/Summer solstice.
        return int( solar_longitude_after( summer, jdn ) + m_locale.lon / 360 );
    case OFID_sequinox: // Southward/Autumn equinox.
        return int( solar_longitude_after( autumn, jdn ) + m_locale.lon / 360 );
    case OFID_ssolstice: // Soutern/Winter solstice.
        return int( solar_longitude_after( winter, jdn ) + m_locale.lon / 360 );
    case OFID_newmoon:
        return int( new_moon_at_or_after( jdn ) + m_locale.lon / 360 );
    }
    return f_invalid;
}

Field Cal::Base::get_opt_field( const Field* fields, Field jdn, int index ) const
{
    if ( index >= int( record_size() ) && index < int( extended_size() ) ) {
        OptFieldID id = opt_index_to_id( index );
        return get_opt_field( fields, jdn, id );
    }
    return f_invalid;
}

bool Base::set_fields_as_next_optional(
    Field* fields, Field jdn, const Field* mask, size_t index ) const
{
    if( index >= (extended_size() - m_opt_fields.size() ) ) {
        OptFieldID id = m_opt_fields[index-record_size()];
        switch( id )
        {
        case OFID_wday:
            // Adjust jdn and knext for week starting Monday 
		    if( mask[index] >= 1 && mask[index] <= 7 && jdn != f_invalid ) {
			    Field knext = kday_on_or_after( Weekday( mask[index] - 1 ), jdn );
			    if( knext != jdn ) {
				    set_fields( fields, knext );
				    return true;
			    }
		    }
            break;
        case OFID_wsday:
		    if( mask[index] >= 1 && mask[index] <= 7 && jdn != f_invalid ) {
                // Adjust jdn and knext for week starting Sunday 
			    Field knext = kday_on_or_after( Weekday( mask[index] - 1 ), jdn + 1 ) - 1;
			    if( knext != jdn ) {
				    set_fields( fields, knext );
				    return true;
			    }
		    }
            break;
        }
        return false;
    }
    return false;
}

bool Base::set_fields_as_prev_optional( Field* fields, Field jdn, const Field* mask, size_t index ) const
{
    if( index >= (extended_size() - m_opt_fields.size() ) ) {
        OptFieldID id = m_opt_fields[index-record_size()];
        switch( id )
        {
        case OFID_wday:
		    if( mask[index] >= 1 && mask[index] <= 7 && jdn != f_invalid ) {
			    Field knext = kday_on_or_before( Weekday( mask[index] - 1 ), jdn );
			    if( knext != jdn ) {
				    set_fields( fields, knext );
				    return true;
			    }
		    }
            break;
        case OFID_wsday:
		    if( mask[index] >= 1 && mask[index] <= 7 && jdn != f_invalid ) {
                // Adjust jdn and knext for week starting Sunday 
			    Field knext = kday_on_or_before( Weekday( mask[index] - 1 ), jdn + 1 ) - 1;
			    if( knext != jdn ) {
				    set_fields( fields, knext );
				    return true;
			    }
		    }
            break;
        }
        return false;
    }
    return false;
}

BoolVec Base::mark_balanced_fields(
    Field* left, Field ljdn, Field* right, Field rjdn, const XRefVec& rank ) const
{
    BoolVec mask( extended_size(), true );
    for ( size_t i = rank.size() - 1; i > 0; --i ) {
        if ( rank[i] < 0 ) {
            break;
        }
        size_t j = rank[i];

        Field l = get_field( left, ljdn, j);
        Field first = get_field_first( left, ljdn, j );
        if ( l != first ) {
            break;
        }

        Field r = get_field( right, rjdn, j );
        Field last = get_field_last( right, rjdn, j );
        if ( r != last ) {
            break;
        }
        mask[j] = false;
    }
    return mask;
}

BoolVec Cal::Base::mark_boundary_fields(
    Field* left, Field ljdn, Field* right, Field rjdn, const XRefVec& rank ) const
{
    BoolVec mask( extended_size(), true );
    if ( left[0] == f_minimum ) {
        if ( right[0] == f_maximum ) {
            return mask;
        }
        for ( size_t i = rank.size() - 1; i > 0; --i ) {
            if ( rank[i] < 0 ) {
                break;
            }
            size_t j = rank[i];

            Field r = get_field( right, rjdn, j );
            Field last = get_field_last( right, rjdn, j );
            if ( r != last ) {
                break;
            }
            mask[j] = false;
        }
        return mask;
    }
    if ( right[0] == f_maximum ) {
        for ( size_t i = rank.size() - 1; i > 0; --i ) {
            if ( rank[i] < 0 ) {
                break;
            }
            size_t j = rank[i];

            Field l = get_field( left, ljdn, j );
            Field first = get_field_first( left, ljdn, j );
            if ( l != first ) {
                break;
            }
            mask[j] = false;
        }
    }
    return mask;
}

Field Cal::Base::get_field( const Field* fields, Field jdn, size_t index ) const
{
    if ( index < record_size() ) {
        return fields[index];
    }
    if ( index < extended_size() ) {
        OptFieldID id = m_opt_fields[index - record_size()];
        return get_opt_field( fields, jdn, id );
    }
    return f_invalid;
}

Field Cal::Base::get_field_first( const Field* fields, Field jdn, size_t index ) const
{
    if ( index < record_size() ) {
        return get_rec_field_first( fields, index );
    }
    if ( index < extended_size() ) {
        OptFieldID id = m_opt_fields[index - record_size()];
        return get_opt_field_first( fields, jdn, id );
    }
    return f_invalid;
}

Field Cal::Base::get_field_last( const Field* fields, Field jdn, size_t index ) const
{
    if ( index < record_size() ) {
        return get_rec_field_last( fields, index );
    }
    if ( index < extended_size() ) {
        OptFieldID id = m_opt_fields[index - record_size()];
        return get_opt_field_last( fields, jdn, id );
    }
    return f_invalid;
}

Field Base::get_rec_field_first( const Field* fields, size_t index ) const
{
    return 1;
}

Field Base::get_rec_field_last( const Field* fields, size_t index ) const
{
    return f_invalid;
}

Field Cal::Base::get_opt_field_first( const Field* fields, Field jdn, OptFieldID id ) const
{
    return 1;
}

Field Cal::Base::get_opt_field_last( const Field* fields, Field jdn, OptFieldID id ) const
{
    switch ( id )
    {
    case OFID_wday:
    case OFID_wsday:
        return 7;
    }
    return f_invalid;
}

Field Base::unit_is_int( const Field* fields, Unit unit ) const
{
    // We can always add years and multiple days to the fields without problem.
    switch( unit )
    {
    case unit_day:
        return 1;
    case unit_week:
        return 7;
    }
    return 0;
}

bool Base::can_add_unit( const Field* fields, Unit unit ) const
{
    // We can always add years and multiple days to the fields without problem.
    return unit == unit_month ? false : true;
}

bool Base::add_to_fields( Field* fields, Field value, Unit unit ) const
{
    return false;
}

double Base::get_average_days( const Field* fields, Unit unit ) const
{
    const double tropical_year = 365.24219;

    switch( unit )
    {
    case unit_year:
        return tropical_year;
    case unit_month:
        return tropical_year / 12;
    }
    // We only deal with non-integer units
    return 0.0;
}

Field Base::compare_minor_fields( const Field* left, const Field* right, size_t index )
{
    for( size_t i = index + 1 ; i < record_size() ; i++ ) {
        if( left[i] == f_invalid || right[i] == f_invalid ) {
            return f_invalid;
        }
        if( left[i] < right[i] ) {
            return 1;
        }
        if( left[i] > right[i] ) {
            return -1;
        }
    }
    return 0;
}

string Base::lookup_token( Field field, const string& vcode, bool abbrev ) const
{
    return get_grammar()->lookup_token( field, vcode, abbrev );
}

string Base::get_alias_fieldname( const string& alias ) const
{
    return get_grammar()->get_field_alias( alias );
}

void Base::get_input_info( SchemeFormatInfo* info ) const
{
    get_grammar()->get_input_formats( info, get_input_fcode() );
}

void Base::get_output_info( SchemeFormatInfo* info ) const
{
    get_grammar()->get_output_formats( info, get_output_fcode() );
}

string Base::get_input_fcode() const
{
    return m_input_fcode.empty() ? "def" : m_input_fcode;
}

string Base::get_output_fcode() const
{
    return m_output_fcode.empty() ? "def" : m_output_fcode;
}

Grammar* Base::get_grammar() const
{
    if( m_grammar == NULL ) {
        create_default_grammar();
    }
    return m_grammar;
}

Format* Base::get_format( const string& fcode ) const
{
    Format* fmt = get_grammar()->get_format( fcode );
    if( fmt == NULL && fcode == "u" ) {
        fmt = m_grammar->create_format_unit();
    }
    return fmt;
}

void Base::set_grammar( Grammar* grammar )
{
    if( grammar == NULL || m_grammar != NULL ) {
        return;
    }
    m_grammar = grammar;
    set_output_fcode( grammar->get_pref_output_fcode() );
    set_input_fcode( grammar->get_pref_input_fcode() );
    StringVec optfields = grammar->get_opt_fieldnames();
    for( size_t i = 0 ; i < optfields.size() ; i++ ) {
        add_opt_field( optfields[i] );
    }
}

void Base::add_opt_field( const string& fieldname )
{
    OptFieldID id = get_opt_field_id( fieldname );
    if( id != OFID_NULL ) {
        m_opt_fields.push_back( id );
    }
}

XRefVec Base::get_xref_order( int cnt, const FormatText* fmt ) const
{
    XRefSet xref_set;
    string format = fmt->get_user_input_str();
    XRefMap::const_iterator it = m_xref_inputs.find( format );
    if( it == m_xref_inputs.end() ) {
        // Does not exist yet.
        xref_set = create_input_xref_set( fmt );
        m_xref_inputs[format] = xref_set; // Add it for next time
    } else {
        xref_set = it->second;
    }

    XRefSet::const_iterator it2 = xref_set.find( cnt );
    if( it2 == xref_set.end() ) {
        return XRefVec(0);
    }
    return it2->second;
}

FieldVec Base::fields_to_vec( const Field* fields ) const
{
    size_t size = record_size();
    FieldVec f(size);
    for( size_t i = 0 ; i < size ; i++ ) {
        f[i] = fields[i];
    }
    return f;
}

bool Base::is_complete( const Field* fields ) const
{
    size_t size = record_size();
    for( size_t i = 0 ; i < size ; i++ ) {
        if( fields[i] == f_invalid ) {
            return false;
        }
    }
    return true;
}

void Base::copy_fields( Field* to, const Field* from ) const
{
    size_t size = record_size();
    for( size_t i = 0 ; i < size ; i++ ) {
        to[i] = from[i];
    }
}

Field Base::compare_except( const Field* first, const Field* second, size_t except ) const
{
    size_t size = record_size();
    for( size_t i = 0 ; i < size ; i++ ) {
        if( i == except ) {
            continue;
        }
        if( first[i] == f_invalid || second[i] == f_invalid ) {
            return f_invalid;
        }
        if( first[i] != second[i] ) {
            return second[i] - first[i];
        }
    }
    return 0;
}

int Base::opt_id_to_index( OptFieldID id ) const
{
    for( size_t i = 0 ; i < m_opt_fields.size() ; i++ ) {
        if( m_opt_fields[i] == id ) {
            return record_size() + i;
        }
    }
    return -1;
}

XRefVec Base::get_default_xref_order( int count ) const
{
    XRefVec xref( record_size(), -1 );
    for( size_t i = 0 ; i < record_size() ; i++ ) {
        if( i == count ) {
            break;
        }
        xref[i] = i;
    }
    return xref;
}

int Base::get_ymd_fieldname_index( const string& fieldname ) const
{
    for( size_t i = 0 ; i < s_sizeof_ymd_fieldnames ; i++ ) {
        if( fieldname == s_ymd_fieldnames[i] ) {
            return i;
        }
    }
    return -1;
}

string Base::get_ymd_fieldname( size_t index ) const
{
    if( index < s_sizeof_ymd_fieldnames ) {
        return s_ymd_fieldnames[index];
    }
    return "";
}

int Base::get_opt_fieldname_index( const string& fieldname ) const
{
    for( size_t i = 0 ; i < m_opt_fields.size() ; i++ ) {
        if( get_opt_fieldname( m_opt_fields[i] ) == fieldname ) {
            return record_size() + i;
        }
    }
    return -1;
}

bool Base::is_tier1( const string& fieldname, const FormatText* fmt ) const
{
    return fmt->is_tier1( fieldname );
}

void Base::create_default_grammar() const
{
    if( m_grammar == NULL ) {
        m_grammar = new Grammar( "" );
    }
    FormatText* fmt = m_grammar->create_format_text( "def" );
    string format;
    for( size_t i = 0 ; i < extended_size() ; i++ ) {
        if( i > 0 ) {
            format += "| ";
        }
        format += "(" + get_fieldname( i ) + ")";
    }
    fmt->set_control( format );
}

XRefVec Base::create_xref( const StringVec& fieldnames ) const 
{
    size_t size = extended_size();
    XRefVec xref( size, -1 );
    std::vector<bool> used( size, false );
    for( size_t i = 0, u = 0 ; i < size ; i++ ) {
        if( i < fieldnames.size() ) {
            int index = get_fieldname_index( fieldnames[i] );
            if( index >= 0 && index < int(size) ) {
                xref[i] = index;
                used[index] = true;
            }
        } else {
            while( u < size && used[u] ) {
                u++;
            }
            if( u < size ) {
                xref[i] = u;
                u++;
            }
        }
    }
    return xref;
}

XRefSet Base::create_input_xref_set( const FormatText* fmt ) const
{
    XRefVec order = create_xref( fmt->get_input_fields() );
    StringVec rank_fns = fmt->get_rank_fieldnames();
    if( rank_fns.empty() ) {
        rank_fns = get_rank_fieldnames();
    }
    XRefVec rank = create_xref( rank_fns );

    XRefSet xrefset;
    size_t cnt = order.size();
    xrefset[cnt] = order;
    while( cnt > 1 ) {
        --cnt;
        XRefVec x;
        for( size_t i = 0 ; i < order.size() ; i++ ) {
            if( rank[cnt] != order[i] ) {
                x.push_back( order[i] );
            }
        }
        assert( x.size() <= order.size() );
        xrefset[cnt] = x;
        order = x;
    }
    return xrefset;
}

// End of src/cal/calbase.cpp file
