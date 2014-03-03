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

using namespace Cal;
using namespace std;

Base::Base()
    : m_grammar(NULL), m_current_order(0), m_current_format(0)
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
    return get_extended_fieldname_index( fieldname );
}

string Base::get_fieldname( size_t index ) const
{
    const char* fnames[] = { "year", "month", "day" };
    if( index < 3 ) {
        return fnames[index];
    }
    return get_extended_fieldname( index );
}

Field Base::get_extended_field( const Field jdn, size_t index ) const
{
    if( index == record_size() + EFN_wday && jdn != f_invalid ) {
        return ( jdn % 7 ) + 1;
    }
    return f_invalid;
}

bool Base::check_usable( const Field* fields ) const
{
    if( fields[0] == f_invalid ) {
        return false;
    }
    bool found_invalid = false;
    for( size_t i = 1 ; i < record_size() ; i++ ) {
        if( found_invalid ) {
            if( fields[i] != f_invalid ) {
                return false;
            }
        } else {
            if( fields[i] == f_invalid ) {
                found_invalid = true;
            }
        }
    }
    return true;
}

bool Base::balance_fields( Field* firsts, Field* lasts ) const
{
    for( size_t i = 1 ; i < record_size() ; i++ ) {
        if( firsts[i] == f_invalid && lasts[i] == f_invalid ) {
            return true; // Balanced
        }
        if( ( firsts[i] == f_invalid && lasts[i] != f_invalid ) ||
            ( firsts[i] != f_invalid && lasts[i] == f_invalid )
        ) {
            break; // Unbalanced
        }
    }
    bool ret = true;
    for( size_t i = 1 ; i < record_size() ; i++ ) {
        if( firsts[i] == f_invalid && lasts[i] == f_invalid ) {
            break; // Now balanced
        }
        if( firsts[i] == f_invalid && lasts[i] != f_invalid ) {
            ret = set_field_first( firsts, i );
        } else if( firsts[i] != f_invalid && lasts[i] == f_invalid ) {
            ret = set_field_last( lasts, i );
        }
        if( ret == false ) {
            return false;
        }
    }
    return true;
}

bool Base::set_field_first( Field* fields, size_t index ) const
{
    fields[index] = 1;
    return true;
}

bool Base::set_field_last( Field* fields, size_t index ) const
{
    fields[index] = get_field_last( fields, index );
    return ( fields[index] != f_invalid );
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

StringVec Base::get_orders()
{
    StringVec orders;
    string order;
    if( m_grammar && m_grammar->order_size() ) {
        for( size_t i = 0 ; i < m_grammar->order_size() ; i++ ) {
            order = m_grammar->order( i );
            size_t pos = order.find( " |" );
            if( pos != string::npos ) {
                order = order.substr( 0, pos );
            }
            orders.push_back( order );
        }
    } else {
        order = create_default_order();
        orders.push_back( order );
    }
    return orders;
}

StringVec Base::get_formats()
{
    StringVec formats;
    Format* fmt;
    if( m_grammar && m_grammar->format_size() ) {
        for( size_t i = 0 ; i < m_grammar->format_size() ; i++ ) {
            fmt = m_grammar->get_format( i );
            string format = fmt->get_user_format( m_grammar );
            formats.push_back( format );
        }
    } else {
        Format deffmt( create_default_format() );
        string format = deffmt.get_user_format( m_grammar );
        formats.push_back( format );
    }
    return formats;
}

string Base::get_format()
{
    if( m_grammar && m_grammar->format_size() ) {
        return m_grammar->format( m_current_format );
    }
    return create_default_format();
}

void Base::set_grammar( Grammar* grammar )
{
    m_grammar = grammar;
    set_current_order( grammar->get_pref_order() );
    m_current_format = grammar->get_pref_format();
}

void Base::set_current_order( int index )
{
    string order;
    if( m_grammar && m_grammar->order_size() ) {
        m_current_order = index;
        order = m_grammar->order( index );
    } else {
        m_current_order = 0;
        order = create_default_order();
    }
    m_xref_order.clear();
    XRefVec xref;
    while( order.size() ) {
        string fname = get_first_word( order, &order );
        if( fname == "|" ) {
            m_xref_order[xref.size()] = xref;
            xref.clear();
            continue;
        }
        int i = get_fieldname_index( get_alias_fieldname( fname ) );
        xref.push_back( i );
    }
    m_xref_order[xref.size()] = xref;
}

void Base::set_current_format( int index )
{
    if( index < 1 || m_grammar == NULL || index > (int) m_grammar->format_size() ) {
        m_current_format = 0;
    } else {
        m_current_format = index;
    }
}

XRefVec Base::get_xref_order( int cnt )
{
    if( m_xref_order.count( cnt ) == 0 ) {
        XRefVec x;
        return x;
    }
    return m_xref_order[cnt];
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

int Base::get_extended_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "wday" ) {
        return record_size() + EFN_wday;
    }
    return -1;
}

string Base::get_extended_fieldname( size_t index ) const
{
    if( index == record_size() + EFN_wday ) {
        return "wday";
    }
    return "";
}

string Base::create_default_order()
{
    string order;
    for( size_t i = record_size() ; i > 0 ; --i ) {
        if( i < record_size() ) {
            order += " | ";
        }
        for( size_t j = 0 ; j < i ; j++ ) {
            if( j > 0 ) {
                order += " ";
            }
            order += get_fieldname( j );
        }
    }
    return order;
}

string Base::create_default_format()
{
    string format;
    for( size_t i = 0 ; i < record_size() ; i++ ) {
        format += "@";
        if( i > 0 ) {
            format += " ";
        }
        format += "(" + field_to_str( i+1 ) + ":" + get_fieldname( i ) + ")";
    }
    return format;
}

// End of src/cal/calbase.cpp file
