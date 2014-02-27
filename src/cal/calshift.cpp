/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calshift.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Shift variant calendar implimentation.
 * Author:      Nick Matthews
 * Website:     http://pcalcalc.org
 * Created:     22nd September 2013
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

#include "calshift.h"

#include "calparse.h"
#include "calrecord.h"
#include "calscheme.h"
#include "calschemes.h"

#include <cassert>

using namespace Cal;
using namespace std;

Shift::Shift( Schemes* schs, const string& data )
    : m_base(NULL), Base()
{
    string body;
    string word = get_first_word( data, &body );
    Scheme* sch = schs->get_scheme( word );
    assert( sch != NULL );
    m_base = sch->get_base();
    assert( m_base != NULL );
    assert( record_size() == 3 );
    Field jdn = str_to_field( body );
    Record rec( m_base, jdn );
    m_start_era = rec.get_fieldvec();
    rec.set_jdn( jdn-1 );
    m_before_era = rec.get_fieldvec();
}

Shift::~Shift()
{
}

int Shift::get_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "unshift" ) { // Unshifted value
        return extended_size() - 1;
    }
    return m_base->get_fieldname_index( fieldname );
}

string Shift::get_fieldname( size_t index ) const
{
    if( index == extended_size() - 1 ) {
        return "unshift";
    }
    return m_base->get_fieldname( index );
}

Field Shift::get_jdn( const Field* fields ) const
{
    if( is_complete( fields ) == false ) {
        return f_invalid;
    }
    FieldVec f = get_vec_adjusted_to_base( fields );
    return m_base->get_jdn( &f[0] );
}

Field Shift::get_extended_field( const Field jdn, size_t index ) const
{
    if( index == extended_size() - 1 ) {
        // "unshift" return unshifted value
        Record rec( m_base, jdn );
        return rec.get_field( 0 );
    }
    return f_invalid;
}

bool Shift::check_usable( const Field* fields ) const
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

Field Shift::get_field_last( const Field* fields, size_t index ) const
{
    assert( false ); // This should not be called - handled by balance_fields.
    return f_invalid;
}

bool Shift::set_fields_as_begin_first( Field* fields, const Field* mask )
{
    copy_fields( fields, mask );
    if( mask[0] == f_invalid ) {
        return false;
    }
    if( mask[2] == f_invalid ) {
        if( mask[1] == f_invalid ) {
            fields[1] = m_start_era[1];
            fields[2] = m_start_era[2];
            return true;
        }
        if( mask[1] == m_start_era[1] ) {
            fields[2] = m_start_era[2];
            return true;
        }
        fields[0] += m_start_era[0];
        if( mask[1] < m_start_era[1] ) {
            fields[0] += 1;
        }
        bool ret = m_base->set_fields_as_begin_first( &fields[0], &fields[0] );
        fields[0] = get_adjusted_to_shift( fields );
        return ret;
    }
    if( mask[1] == f_invalid ) {
        return false;
    }
    return true;
}

bool Shift::set_fields_as_next_first( Field* fields, const Field* mask )
{
    // There is a second range only when
    // mask month == m_start_era month and mask day == f_invalid
    if( mask[1] != m_start_era[1] || mask[2] != f_invalid  ) {
        return false;
    }
    if( fields[2] != m_start_era[2] ) {
        return false;
    }
    fields[0] += m_start_era[0] + 1;
    fields[2] = m_base->get_field_first( mask, 2 );
    fields[0] = get_adjusted_to_shift( fields );
    return true;
}

bool Shift::set_fields_as_begin_last( Field* fields, const Field* mask )
{
    copy_fields( fields, mask );
    if( mask[0] == f_invalid ) {
        return false;
    }
    if( mask[2] == f_invalid ) {
        if( mask[1] == f_invalid ) {
            fields[1] = m_before_era[1];
            fields[2] = m_before_era[2];
            return true;
        }
        fields[0] += m_before_era[0];
        if( mask[1] < m_start_era[1] ) {
            fields[0] += 1;
        }
        bool ret = m_base->set_fields_as_begin_last( &fields[0], &fields[0] );
        fields[0] = get_adjusted_to_shift( fields );
        return ret;
    }
    if( mask[1] == f_invalid ) {
        return false;
    }
    return true;
}

bool Shift::set_fields_as_next_last( Field* fields, const Field* mask )
{
    // There is a second range only when
    // mask month == m_before_era month and mask day == f_invalid
    if( mask[1] != m_before_era[1] || mask[2] != f_invalid  ) {
        return false;
    }
    if( fields[2] == m_before_era[2] ) {
        // already done
        return false;
    }
    fields[0] += m_start_era[0] + 1;
    fields[2] = m_before_era[2];
    fields[0] = get_adjusted_to_shift( fields );
    return true;
}

void Shift::remove_fields_if_first( Field* fields ) const
{
    if( fields[1] == m_start_era[1] && fields[2] == m_start_era[2] ) {
        // year start
        fields[1] = fields[2] = f_invalid;
        return;
    }
    // We will assume all field start values are 1
    if( fields[1] == m_start_era[1] && m_start_era[2] != 1 ) {
        // don't mess with the new year month
        return;
    }
    if( fields[2] == 1 ) {
        fields[2] = f_invalid;
    }
}

void Shift::remove_fields_if_last( Field* fields ) const
{
    if( fields[1] == m_before_era[1] && fields[2] == m_before_era[2] ) {
        // year start
        fields[1] = fields[2] = f_invalid;
        return;
    }
    FieldVec fs = get_vec_adjusted_to_base( fields );
    Field f = m_base->get_field_last( &fs[0], 2 );

    if( fields[1] == m_before_era[1] && m_before_era[2] != f ) {
        // don't mess with the new year month
        return;
    }
    if( fields[2] == f ) {
        fields[2] = f_invalid;
    }
}

bool Shift::balance_fields( Field* firsts, Field* lasts ) const
{
    // assume first field values = 1
    if( firsts[0] == f_invalid || lasts[0] == f_invalid ) {
        // both need the major count
        return false;
    }
    if( firsts[1] == f_invalid && lasts[1] != f_invalid ||
        firsts[1] != f_invalid && lasts[1] == f_invalid 
    ) {
        return false;
    }
    if( firsts[1] == m_start_era[1] || lasts[1] == m_before_era[1] ) {
        // Don't mess if either field is start/end of year
        return false;
    }
    if( firsts[2] == f_invalid && lasts[2] != f_invalid ) {
        firsts[2] = 1;
    } else if( firsts[2] != f_invalid && lasts[2] == f_invalid ) {
        FieldVec fs = get_vec_adjusted_to_base( lasts );
        Field f = m_base->get_field_last( &fs[0], 2 );
        lasts[2] = f;
    }
    return true;
}

bool Shift::set_field_first( Field* fields, size_t index ) const 
{
    assert( false ); // This should not be called - handled by balance_fields.
    return false;
}

bool Shift::set_field_last( Field* fields, size_t index ) const 
{
    assert( false ); // This should not be called - handled by balance_fields.
    return false;
}

void Shift::set_fields( Field* fields, Field jdn ) const
{
    m_base->set_fields( &fields[0], jdn );
    fields[0] = get_adjusted_to_shift( fields );
}

Field Shift::unit_is_int( const Field* fields, Unit unit ) const
{
    return m_base->unit_is_int( fields, unit );
}

bool Shift::can_add_unit( const Field* fields, Unit unit ) const
{
    return m_base->can_add_unit( fields, unit );
}

double Shift::get_average_days( const Field* fields, Unit unit ) const
{
    return m_base->get_average_days( fields, unit );
}

bool Shift::add_to_fields( Field* fields, Field value, Unit unit ) const
{
    FieldVec fs = get_vec_adjusted_to_base( fields );
    bool ret = m_base->add_to_fields( &fs[0], value, unit );
    if( ret == true ) {
        copy_fields( fields, &fs[0] );
        fields[0] = get_adjusted_to_shift( fields );
    }
    return ret;
}

bool Shift::normalise( Field* fields, Norm norm ) const
{
    FieldVec fs = get_vec_adjusted_to_base( fields );
    bool ret = m_base->normalise( &fs[0], norm );
    if( ret ) {
        copy_fields( fields, &fs[0] );
        fields[0] = get_adjusted_to_shift( fields );
    }
    return ret;
}

Field Shift::get_adjustment( const Field* fields ) const
{
    for( size_t i = 1 ; i < record_size() ; i++ ) {
        assert( fields[i] != f_invalid );
        if( fields[i] > m_start_era[i] ) {
            return 0;
        }
        if( fields[i] < m_start_era[i] ) {
            return 1;
        }
    }
    return 0; // equal
}

FieldVec Shift::get_vec_adjusted_to_base( const Field* fields ) const
{
    FieldVec fs(m_base->record_size());
    fs[0] = get_adjusted_to_base( fields );
    for( size_t i = 1 ; i < fs.size() ; i++ ) {
        fs[i] = fields[i];
    }
    return fs;
}

Field Shift::get_adjusted_to_base( const Field* fields ) const
{
    return fields[0] + m_start_era[0] + get_adjustment( fields );
}

Field Shift::get_adjusted_to_shift( const Field* fields ) const
{
    return fields[0] - m_start_era[0] - get_adjustment( fields );
}

// End of src/cal/calshift.cpp file
