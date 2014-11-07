/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calregnal.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Regnal variant calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th March 2014
 * Copyright:   Copyright (c) 2014, Nick Matthews.
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

#include "calregnal.h"

#include "cal/calendars.h"
#include "calparse.h"
#include "calrecord.h"
#include "calscheme.h"

#include <cassert>

using namespace Cal;
using namespace std;


Regnal::Regnal( 
    const StringVec& fieldnames,
    const FieldVec& fixedfields,
    const vector<RegnalEra>& eras
    ) : m_eras(eras), m_fieldnames(fieldnames), m_fixed_fields(fixedfields),
    m_rec_size(fieldnames.size()+1), Base()
{
}

Regnal::~Regnal()
{
    for( size_t i = 1 ; i < m_eras.size() ; i++ ) {
        if( m_eras[i].local ) {
            delete m_eras[i].scheme;
        }
    }
}

bool Regnal::is_ok() const
{
    return m_rec_size > 1 && m_eras.size() > 1; 
}

int Regnal::get_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "era" ) {
        return 0;
    }
    for( size_t i = 0 ; i < m_fieldnames.size() ; i++ ) {
        if( m_fieldnames[i] == fieldname ) {
            return i + 1;
        }
    }
    return -1;
}

string Regnal::get_fieldname( size_t index ) const
{
    if( index == 0 ) {
        return "era";
    }
    if( index >= m_fieldnames.size() + 1 ) {
        return "";
    }
    return m_fieldnames[index-1];
}

Field Regnal::get_jdn( const Field* fields ) const
{
    FieldVec fs = get_base_fields( fields );
    if( fs.size() == 0 ) {
        return f_invalid;
    }
    return m_eras[fields[0]].base->get_jdn( &fs[0] );
}

void Regnal::set_fixed_fields( Field* fields ) const
{
    // Note, the field is only set if its current value is f_invalid.
    // m_fixed_fields.size() may be zero.
    for( size_t i = 0 ; i < m_fixed_fields.size() ; i++ ) {
        if( m_fixed_fields[i] != f_invalid && fields[i] == f_invalid ) {
            fields[i] = m_fixed_fields[i];
        }
    }
}

bool Regnal::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    Field e = mask[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        return false;
    }
    if( mask[1] == f_invalid ) {
        Record rec( this, m_eras[e].begin );
        copy_fields( fields, rec.get_field_ptr() );
        return true;
    }
    FieldVec m = get_base_fields( mask );
    Record rec( m_eras[e].base );
    if( rec.set_fields_as_begin_first( &m[0] ) ) {
        return make_regnal_fields( fields, e, rec );
    }
    return false;
}

bool Regnal::set_fields_as_next_first( Field* fields, const Field* mask ) const
{
    Field e = mask[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        return false;
    }
    if( mask[1] == f_invalid ) {
        return false;
    }
    FieldVec m = get_base_fields( mask );
    FieldVec f = get_base_fields( fields );
    Record rec( m_eras[e].base, &f[0], f.size() );
    if( rec.set_fields_as_next_first( &m[0] ) ) {
        if( rec.get_jdn() > m_eras[e].end ) {
            return false;
        }
        return make_regnal_fields( fields, e, rec );
    }
    return false;
}

bool Regnal::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    Field e = mask[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        return false;
    }
    if( mask[1] == f_invalid ) {
        Record rec( this, m_eras[e].end );
        copy_fields( fields, rec.get_field_ptr() );
        return true;
    }
    FieldVec m = get_base_fields( mask );
    Record rec( m_eras[e].base );
    if( rec.set_fields_as_begin_last( &m[0] ) ) {
        return make_regnal_fields( fields, e, rec );
    }
    return false;
}

bool Regnal::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    Field e = mask[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        return false;
    }
    if( mask[1] == f_invalid ) {
        return false;
    }
    FieldVec m = get_base_fields( mask );
    FieldVec f = get_base_fields( fields );
    Record rec( m_eras[e].base, &f[0], f.size() );
    if( rec.set_fields_as_next_last( &m[0] ) ) {
        if( rec.get_jdn() > m_eras[e].end ) {
            return false;
        }
        return make_regnal_fields( fields, e, rec );
    }
    return false;
}

void Regnal::remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const
{
    for( size_t i = m_eras.size() - 1 ; i > 0  ; --i ) {
        if( ljdn > m_eras[i].begin ) {
            break;
        }
        if( ljdn == m_eras[i].begin && rjdn == m_eras[i].end ) {
            left[0] = right[0] = i;
            for( size_t j = 1 ; j < m_rec_size ; j++ ) {
                left[j] = right[j] = f_invalid;
            }
            return;
        }
    }
    if( left[0] != right[0] || left[0] == f_invalid ) {
        return;
    }
    size_t e = left[0];
    Base* base = m_eras[e].base;
    Record lrec( base, ljdn );
    Record rrec( base, rjdn );
    lrec.remove_balanced_fields( &rrec );
    make_regnal_fields( left, e, lrec );
    make_regnal_fields( right, e, rrec );
}

void Regnal::set_fields( Field* fields, Field jdn ) const
{
    Field e = 0;
    for( size_t i = m_eras.size() - 1 ; i > 0  ; --i ) {
        if( jdn >= m_eras[i].begin && jdn <= m_eras[i].end ) {
            e = i;
            break;
        }
    }
    Record rec( m_eras[e].base, jdn );
    make_regnal_fields( fields, e, rec );
    return;
}

FieldVec Regnal::get_base_fields( const Field* fields ) const
{
    Field e = fields[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        // Return empty vector to show error
        FieldVec fs;
        return fs;
    }
    Base* base = m_eras[e].base;
    size_t fs_size = base->record_size();
    FieldVec fs( fs_size, f_invalid );
    for( size_t i = 1 ; i < m_rec_size ; i++ ) {
        int index = m_eras[e].xref[i-1];
        if( index >= 0 && index < (int) fs_size ) {
            fs[index] = fields[i];
        }
    }
    base->set_fixed_fields( &fs[0] );
    return fs;
}

bool Regnal::make_regnal_fields( Field* fields, Field era, Record& rec ) const
{
    fields[0] = era;
    size_t size = m_rec_size - 1;
    for( size_t i = 0 ; i < size ; i++ ) {
        int x = m_eras[era].xref[i];
        if( x >= 0 ) {
            fields[i+1] = rec.get_field( x );
        }
    }
    return true;
}

// End of src/cal/calregnal.cpp
