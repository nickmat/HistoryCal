/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calbase.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base class, used as base for all calendar calulations.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
 * Copyright:   Copyright (c) 2013-2015, Nick Matthews.
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

#include "calformat.h"
#include "calgrammar.h"
#include "calparse.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;

const char* Base::s_ymd_fieldnames[] = { "year", "month", "day" };
size_t Base::s_sizeof_ymd_fieldnames = sizeof(s_ymd_fieldnames) / sizeof(const char*);

Base::Base()
    : m_grammar(NULL)
{
}

Base::~Base()
{
    // Default grammar has no code
    if( m_grammar && m_grammar->code() == "" ) {
        delete m_grammar;
    }
}

void Base::remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const
{
    size_t i;
    for( i = record_size() - 1 ; i > 0 ; --i ) {
        if( left[i] == f_invalid || right[i] == f_invalid ) {
            return; // Must be fully qualified
        }
        Field l = get_field_first( left, i );
        Field r = f_invalid;
        if( l == left[i] ) {
            r = get_field_last( right, i );
        }
        if( r != right[i] ) {
            break;
        }
    }
    for( i++ ; i < record_size() ; i++ ) {
        left[i] = right[i] = f_invalid;
    }
}

Field Base::get_field_first( const Field* fields, size_t index ) const
{
    return 1;
}

Field Base::get_field_last( const Field* fields, size_t index ) const
{
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

bool Base::resolve_input(
    Field* fields, const InputFieldVec& input, const string& format_code ) const
{
    string fcode;
    if( format_code.empty() ) {
        fcode = get_input_fcode();
    } else {
        fcode = format_code;
    }
    size_t cnt = 0;
    FieldVec fs( extended_size(), f_invalid );
    Format* fmt = get_grammar()->get_format( fcode );
    if( fmt == NULL ) {
        return false;
    }
    for( size_t i = 0 ; i < input.size() ; i++ ) {
        if( input[i].type == IFT_null ) {
            continue;
        }
        string fname;
        if( input[i].type == IFT_dual2 ) {
            fname = fmt->get_1st_input_field( IFT_dual2 );
            if( fname.empty() ) {
                continue; // Ignore if we can't find it.
            }
        }
        if( input[i].vocab ) {
            fname = input[i].vocab->get_fieldname();
            if( fname.empty() ) {
				fname = fmt->get_input_field( input[i].vocab );
				if( fname.empty() ) {
                    continue; // Give up.
				}
            }
        }
        if( fname.size() ) {
            int index = get_fieldname_index( fname );
            if( index >= (int) record_size() ) {
                // Input an extended field
                fields[index] = input[i].value;
                continue;
            }
        }
        fs[cnt] = input[i].value;
        cnt++;
    }
    if( cnt < 1 ) {
        return false;
    }
    XRefVec xref = get_xref_order( cnt, fcode );
    if( xref.empty() ) {
        return false;
    }
    for( size_t i = 0 ; i < cnt ; i++ ) {
        int x = xref[i];
        if( x >= 0 && x < (int) extended_size() ) {
            fields[x] = fs[i];
        }
    }
    return true;
}

string Base::lookup_token( Field field, const string& vcode, bool abbrev ) const
{
    return get_grammar()->lookup_token( field, vcode, abbrev );
}

string Base::get_alias_fieldname( const string& alias ) const
{
    return get_grammar()->get_field_alias( alias );
}


void Base::get_input_formats( SchemeFormats* input ) const
{
    input->descrip.clear();
    input->code.clear();
    input->current = 0;
    get_grammar()->get_input_formats( input, get_input_fcode() );
}

void Base::get_output_formats( SchemeFormats* output ) const
{
    output->descrip.clear();
    output->code.clear();
    output->current = 0;
    get_grammar()->get_output_formats( output, get_output_fcode() );
}

string Base::get_input_fcode() const
{
    return m_input_fcode.empty() ? "def" : m_input_fcode;
}

string Base::get_output_fcode() const
{
    return m_output_fcode.empty() ? "def" : m_output_fcode;
}

string Base::get_input_order_str( const string& fcode ) const
{
    return get_grammar()->get_input_format( fcode );
}

string Base::get_format_str_for_output() const
{
    string format;
    Format* fmt = get_grammar()->get_format( get_output_fcode() );
    if( fmt ) {
        format = fmt->get_format();
    }
    return format;
}

Grammar* Base::get_grammar() const
{
    if( m_grammar == NULL ) {
        create_default_grammar();
    }
    return m_grammar;
}


void Base::set_grammar( Grammar* grammar )
{
    if( grammar == NULL ) {
        return;
    }
    m_grammar = grammar;
    set_output_fcode( grammar->get_pref_output_fcode() );
    set_input_fcode( grammar->get_pref_input_fcode() );
}

XRefVec Base::get_xref_order( int cnt, const string& fcode ) const
{
    XRefSet xref_set;
    string format = get_input_order_str( fcode );
    XRefMap::const_iterator it = m_xref_inputs.find( format );
    if( it == m_xref_inputs.end() ) {
        // Does not exist yet.
        xref_set = create_input_xref_set( fcode );
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

void Base::create_default_grammar() const
{
    assert( m_grammar == NULL );
    Grammar* gmr = new Grammar( "" );
    Format* fmt = gmr->create_format( "def" );
    string format;
    for( size_t i = 0 ; i < record_size() ; i++ ) {
        if( i > 0 ) {
            format += "| ";
        }
        format += "(" + get_fieldname( i ) + ")";
    }
    fmt->set_format( format );
    m_grammar = gmr;
}

XRefSet Base::create_input_xref_set( const string& fcode ) const
{
    XRefVec xref;
    string order = get_input_order_str( fcode );
    int i, max_j = -1;
    for( i = 0 ; order.size() ; i++ ) {
        string fname = get_first_word( order, &order );
        int j = get_fieldname_index( get_alias_fieldname( fname ) );
        xref.push_back( j );
        if( j > max_j ) {
            max_j = j;
        }
    }
    XRefSet xref_set;
    while( max_j > -1 ) {
        xref_set[xref.size()] = xref;
        int prev_max_j = max_j;
        max_j = -1;
        XRefVec x;
        for( i = 0 ; i < (int) xref.size() ; i++ ) {
            int j = xref[i];
            if( j == prev_max_j ) {
                continue;
            }
            x.push_back( j );
            if( j > max_j ) {
                max_j = j;
            }
        }
        xref = x;
    }
    return xref_set;
}

// End of src/cal/calbase.cpp file
