/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calregnal.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Regnal variant calendar.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     28th March 2014
 * Copyright:   Copyright (c) 2014 ~ 2020, Nick Matthews.
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
    const StringVec& ext_fieldnames,
    const FieldVec& fixedfields,
    const vector<RegnalEra>& eras
    ) : m_eras(eras), m_fieldnames(fieldnames), m_ext_fieldnames(ext_fieldnames),
    m_fixed_fields(fixedfields), Base()
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
    return record_size() > 1 && m_eras.size() > 1;
}

int Cal::Regnal::get_fieldname_index( const std::string & fieldname ) const
{
    for ( size_t i = 0; i < m_ext_fieldnames.size(); i++ ) {
        if ( m_ext_fieldnames[i] == fieldname ) {
            return record_size() + i;
        }
    }
    int index = Base::get_fieldname_index( fieldname );
    if ( index >= int( record_size() ) ) {
        index += m_ext_fieldnames.size();
    }
    return index;
}

Field Regnal::get_jdn( const Field* fields ) const
{
    FieldVec fs = get_base_fields( fields );
    if( fs.size() == 0 ) {
        return f_invalid;
    }
    return m_eras[fields[0]].base->get_jdn( &fs[0] );
}

Field Cal::Regnal::get_opt_field( const Field * fields, Field jdn, int index ) const
{
    assert( index >= int( record_size() ) && index < int( extended_size()) );
    if ( index >= int( record_size() + m_ext_fieldnames.size() ) ) {
        return Base::get_opt_field( fields, jdn, index - m_ext_fieldnames.size() );
    }
    FieldVec fv = get_base_fields( fields );
    size_t e = get_era_index( jdn );
    const Base* base = m_eras[e].base;

//    int i = base->get_fieldname_index( m_ext_fieldnames[index - record_size()] );
    int i = m_eras[e].xref[index-1];
    if ( i >= 0 ) {
        return base->get_opt_field( &fv[0], jdn, i );
    }
    return f_invalid;
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

BoolVec Cal::Regnal::mark_balanced_fields(
    Field* left, Field ljdn, Field* right, Field rjdn, const XRefVec& rank ) const
{
    BoolVec mask( extended_size(), true );


    for ( size_t i = m_eras.size() - 1; i > 0; --i ) {


        if ( ljdn > m_eras[i].begin ) {
            break;
        }
        if ( ljdn == m_eras[i].begin && rjdn == m_eras[i].end ) {
            left[0] = right[0] = i;
            for ( size_t k = 1; k < record_size(); k++ ) {
                mask[k] = false;
            }
            return mask;
        }
    }
    if ( left[0] != right[0] || left[0] == f_invalid ) {
        return mask;
    }
    size_t e = left[0];
    const Base* base = m_eras[e].base;
    Record lrec( base, ljdn );
    Record rrec( base, rjdn );

    XRefVec rank1( rank.size() - 1 );
    for ( size_t i = 0, j = 0; i < rank.size(); i++ ) {
        if ( rank[i] == 0 ) {
            continue;
        }
        rank1[j++] = rank[i] - 1;
    }

    assert( base->record_size() >= rank1.size() );
    BoolVec m = lrec.mark_balanced_fields( rrec, rank1 ); // remove rank[] 0 & decrease others
    m.insert( m.begin(), true );
    return m;
}

void Regnal::set_fields( Field* fields, Field jdn ) const
{
    Field e = get_era_index( jdn );
    Record rec( m_eras[e].base, jdn );
    make_regnal_fields( fields, e, rec );
    return;
}

int Regnal::get_std_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "era" ) {
        return 0;
    }
    int offset = 1;
    for( size_t i = 0 ; i < m_fieldnames.size() ; i++ ) {
        if( m_fieldnames[i] == fieldname ) {
            return i + offset;
        }
    }
    offset += m_fieldnames.size();
    for( size_t i = 0 ; i < m_ext_fieldnames.size() ; i++ ) {
        if( m_ext_fieldnames[i] == fieldname ) {
            return i + offset;
        }
    }
    return -1;
}

string Regnal::get_std_fieldname( size_t index ) const
{
    if( index == 0 ) {
        return "era";
    }
    --index;
    if( index < m_fieldnames.size() ) {
        return m_fieldnames[index];
    }
    index -= m_fieldnames.size();
    if( index < m_ext_fieldnames.size() ) {
        return m_ext_fieldnames[index];
    }
    return "";
}

size_t Regnal::get_era_index( Field jdn ) const
{
    for( size_t i = m_eras.size() - 1 ; i > 0  ; --i ) {
        if( jdn >= m_eras[i].begin && jdn <= m_eras[i].end ) {
            return i;
        }
    }
    return 0;
}

FieldVec Regnal::get_base_fields( const Field* fields ) const
{
    Field e = fields[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        // Return empty vector to show error
        FieldVec fs;
        return fs;
    }
    const Base* base = m_eras[e].base;
    FieldVec fs( extended_size()-1, f_invalid );
    for( size_t i = 0 ; i < m_eras[e].xref.size(); i++ ) {
        int index = m_eras[e].xref[i];
        if( index >= 0 && index < int(fs.size()) ) {
            fs[index] = fields[i + 1];
        }
    }
    base->set_fixed_fields( &fs[0] );
    return fs;
}

bool Regnal::make_regnal_fields( Field* fields, Field era, Record& rec ) const
{
    fields[0] = era;
    size_t size = m_eras[era].base->record_size();
    for( size_t i = 0 ; i < size ; i++ ) {
        int x = m_eras[era].xref[i];
        if( x >= 0 ) {
            fields[i+1] = rec.get_field( x );
        }
    }
    return true;
}

// End of src/cal/calregnal.cpp
