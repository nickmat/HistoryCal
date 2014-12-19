/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calbase.h
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base class, used as base for all calendar calulations.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     21st September 2013
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

#include "calbase.h"

#include "calformat.h"
#include "calgrammar.h"
#include "calparse.h"
#include "calvocab.h"

using namespace Cal;
using namespace std;

Base::Base()
    : m_grammar(NULL)
{
}

int Base::get_fieldname_index( const string& fieldname ) const
{
    // These are the usual fieldnames
    if( fieldname == "year" ) {
        return 0;
    } else if( fieldname == "month" ) {
        return 1;
    } else if( fieldname == "day" ) {
        return 2;
    }
    return -1;
}

string Base::get_fieldname( size_t index ) const
{
    const char* fnames[] = { "year", "month", "day" };
    if( index < 3 ) {
        return fnames[index];
    }
    return "";
}

#if 0
void Base::remove_fields_if_first( Field* fields ) const
{
    for( size_t i = record_size() - 1 ; i < 1  ; --i ) {
        if( get_field_first( fields, i ) == fields[i] ) {
            fields[i] = f_invalid;
        } else {
            return;
        }
    }
}

void Base::remove_fields_if_last( Field* fields ) const
{
    for( size_t i = record_size() - 1 ; i < 1 ; --i ) {
        if( get_field_last( fields, i ) == fields[i] ) {
            fields[i] = f_invalid;
        } else {
            return;
        }
    }
}
#endif

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

string Base::lookup_token( Field field, const string& vcode, bool abbrev ) const
{
    if( m_grammar ) {
        return m_grammar->lookup_token( field, vcode, abbrev );
    }
    return "";
}

string Base::get_alias_fieldname( const string& alias ) const
{
    if( m_grammar ) {
        return m_grammar->get_field_alias( alias );
    }
    return alias;
}


void Base::get_input_formats( SchemeFormats* input ) const
{
    input->descrip.clear();
    input->code.clear();
    input->current = 0;
    if( m_grammar ) {
        m_grammar->get_input_formats( input );
        if( input->code.size() ) {
            return;
        }
    }
    Format deffmt( m_grammar, create_default_format() );
    string format = deffmt.get_order_str();
    input->descrip.push_back( format );
    input->code.push_back( "def" );
}

void Base::get_output_formats( SchemeFormats* output ) const
{
    output->descrip.clear();
    output->code.clear();
    output->current = 0;
    if( m_grammar ) {
        m_grammar->get_output_formats( output );
        if( output->code.size() ) {
            return;
        }
    }
    Format deffmt( m_grammar, create_default_format() );
    string format = deffmt.get_user_format() + "  (def)";
    output->descrip.push_back( format );
    output->code.push_back( "def" );
}

string Base::get_input_format() const
{
    return m_input_format.empty() ? "def" : m_input_format;
}

string Base::get_output_format() const
{
    return m_output_format.empty() ? "def" : m_output_format;
}

string Base::get_format() const
{
    if( m_grammar ) {
        Format* fmt = m_grammar->get_format( m_output_format );
        if( fmt ) {
            return fmt->get_format();
        }
    }
    return create_default_format();
}

void Base::set_grammar( Grammar* grammar )
{
    if( grammar == NULL ) {
        return;
    }
    m_grammar = grammar;
    set_output_format( grammar->get_pref_output_format() );
    set_input_format( grammar->get_pref_input_format() );

    SchemeFormats inputs;
    m_grammar->get_input_formats( &inputs );
    for( size_t i = 0 ; i < inputs.code.size() ; i++ ) {
        add_xref_input( inputs.code[i] );
    }
}

XRefVec Base::get_xref_order( int cnt, const string& fmt ) const
{
    XRefSet xref_set;
    size_t fmtcount = m_xref_inputs.count( fmt );
    if( fmtcount == 0 ) {
        fmtcount = m_xref_inputs.count( m_input_format );
        if( fmtcount == 0 ) {
            return get_default_xref_order( cnt );
        }
        xref_set = m_xref_inputs.find( m_input_format )->second;
    } else {
        xref_set = m_xref_inputs.find( fmt )->second;
    }
    if( xref_set.count( cnt ) == 0 ) {
        return XRefVec(0);
    }
    return xref_set.find( cnt )->second;
}

bool Base::resolve_input(
    Field* fields, const InputFieldVec& input, const string& fmt_code ) const
{
    string code;
    if( fmt_code.empty() ) {
        code = get_input_format();
    } else {
        code = fmt_code;
    }
    size_t cnt = 0;
    FieldVec fs( extended_size(), f_invalid );
    if( m_grammar ) {
        Format* fmt = m_grammar->get_format( code );
        if( fmt == NULL ) {
            return false;
        }
        for( size_t i = 0 ; i < input.size() ; i++ ) {
            if( input[i].type == IFT_null ) {
                continue;
            }
            string fname;
            if( input[i].type == IFT_dual2 ) {
                fname = fmt->get_1st_output_field( IFT_dual2 );
                if( fname.empty() ) {
                    continue; // Ignore if we can't find it.
                }
            }
            if( input[i].vocab ) {
                fname = fmt->get_output_field( input[i].vocab );
                if( fname.empty() ) {
					fname = input[i].vocab->get_fieldname();
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
    } else {
        for( ; cnt < input.size() ; cnt++ ) {
            fs[cnt] = input[cnt].value;
            if( input[cnt].type == IFT_null ) {
                break;
            }
        }
    }
    if( cnt < 1 ) {
        return false;
    }
    XRefVec xref = get_xref_order( cnt, code );
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

string Base::create_default_order() const
{
    string order;
    for( size_t i = 0 ; i < record_size() ; i++ ) {
        if( i > 0 ) {
            order += " ";
        }
        order += get_fieldname( i );
    }
    return order;
}

string Base::create_default_format() const
{
    string format;
    for( size_t i = 0 ; i < record_size() ; i++ ) {
        format += "@";
        if( i > 0 ) {
            format += " ";
        }
        format += "(" + get_fieldname( i ) + ")";
    }
    return format;
}

void Base::add_xref_input( const std::string& code )
{
    if( m_grammar == NULL ) {
        return;
    }
    string order = m_grammar->get_input_format( code );
    XRefVec xref;
    XRefSet xref_set;
    int i, max_j = -1;
    for( i = 0 ; order.size() ; i++ ) {
        string fname = get_first_word( order, &order );
        int j = get_fieldname_index( get_alias_fieldname( fname ) );
        xref.push_back( j );
        if( j > max_j ) {
            max_j = j;
        }
    }
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
    m_xref_inputs[code] = xref_set;
}


// End of src/cal/calbase.cpp file
