/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calera.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Era variant calendar implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     24th September 2013
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

#include "calera.h"

#include "calparse.h"
#include "calrecord.h"
#include "calscheme.h"
#include "calschemes.h"

#include <cassert>

using namespace Cal;
using namespace std;

Era::Era( Schemes* schs, const std::string& data )
    : m_base(NULL), m_rec_size(0), Base()
{
    string body;
    string word = get_first_word( data, &body );
    Scheme* sch = schs->get_scheme( word );
    assert( sch != NULL );
    m_base = sch->get_base();
    assert( m_base != NULL );
    m_rec_size = m_base->record_size() + 1;
    word = get_first_word( body, &body );
    m_offset = m_base->get_fieldname_index( word ) + 1;

    vector<string> eras = parse_statements( peel_cbrackets( body ) );
    for( size_t i = 0 ; i < eras.size() ; i++ ) {
        word = get_first_word( eras[i], &body );
        Field date = str_to_field( word );
        m_epochs.push_back( date );
        Record rec( m_base, date );
        m_edates.push_back( rec.get_fieldvec() );

        date = str_to_field( body );
        m_dates.push_back( date );
    }
}

Era::~Era()
{
}

int Era::get_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "era" ) {
        return 0;
    }
    int ind = m_base->get_fieldname_index( fieldname );
    if( ind >= 0 ) {
        return ind + 1;
    }
    return ind;
}

string Era::get_fieldname( size_t index ) const
{
    if( index == 0 ) {
        return "era";
    }
    return m_base->get_fieldname( index - 1 );
}

Field Era::get_jdn( const Field* fields ) const
{
    FieldVec f = get_base_fields( fields );
    return m_base->get_jdn( &f[1] );
}

bool Era::set_fields_as_begin_first( Field* fields, const Field* mask )
{
    if( mask[0] == f_invalid || mask[0] < 0 || mask[0] > (int) m_epochs.size() ) {
        return false;
    }
    if( mask[m_offset] == f_invalid ) {
        if( mask[0] <= 0 || mask[0] >= (int) m_dates.size()-1 ) {
            return false;
        }
        fields[0] = mask[0];
        m_base->set_fields( &fields[1], m_dates[mask[0]] );
        return make_era_fields( fields );
    }
    assert( m_offset+3 <= m_rec_size ); // assume record ends [year] [month] [day]
    fields[0] = mask[0];
    FieldVec tmask(m_rec_size);
    copy_fields( &tmask[0], mask );
    FieldVec edate = m_edates[mask[0]];
    if( tmask[m_offset+2] == f_invalid ) {
        if( tmask[m_offset+1] == f_invalid ) {
            tmask[m_offset+1] = edate[m_offset+1-1];
            tmask[m_offset+2] = edate[m_offset+2-1];
        } else if( tmask[m_offset+1] == edate[m_offset+1-1] ) {
            tmask[m_offset+2] = edate[m_offset+2-1];
        }
    }
    tmask = get_base_fields( &tmask[0] );
    if( tmask[m_offset+1] == f_invalid ) {
        return false;
    }
    if( m_base->set_fields_as_begin_first( &fields[1], &tmask[1] ) ) {
        return make_era_fields( fields );
    }
    return false;
}

bool Era::set_fields_as_next_first( Field* fields, const Field* mask )
{
    // There is a second range only when
    // mask[m_offset+1] (month) == FieldVecVec[][m_offset-1+1] (month)
    // and mask[m_offset+2] (day) == f_invalid
    if( m_offset+2 >= m_rec_size ) {
        return false;
    }
    size_t era = mask[0];
    FieldVec epoch = m_edates[era];
    if( mask[m_offset+1] != epoch[m_offset+1-1] || mask[m_offset+2] != f_invalid  ) {
        return false;
    }
    // Check we've not already done this.
    if( fields[m_offset+2] != epoch[m_offset+2-1] ) {
        return false;
    }
    FieldVec f = get_base_fields( fields );
    fields[m_offset+2] = m_base->get_field_first( &f[1], m_offset+2-1 );
    return true;
}

bool Era::set_fields_as_begin_last( Field* fields, const Field* mask )
{
    if( mask[0] == f_invalid || mask[0] < 0 || mask[0] > (int) m_epochs.size() ) {
        return false;
    }
    if( mask[m_offset] == f_invalid ) {
        if( mask[0] <= 0 || mask[0] >= (int) m_dates.size()-1 ) {
            return false;
        }
        fields[0] = mask[0];
        m_base->set_fields( &fields[1], m_dates[mask[0]+1]-1 );
        return make_era_fields( fields );
    }

    assert( m_offset+3 <= m_rec_size ); // assume record ends [year] [month] [day]
    fields[0] = mask[0];
    FieldVec tmask(m_rec_size);
    copy_fields( &tmask[0], mask );

    if( tmask[m_offset+2] == f_invalid ) {
        Record rec( m_base, m_epochs[mask[0]] - 1 );
        FieldVec pedate = rec.get_fieldvec();
        if( tmask[m_offset+1] == f_invalid ) {
            tmask[m_offset+1] = pedate[m_offset+1-1];
            tmask[m_offset+2] = pedate[m_offset+2-1];
        } else if( tmask[m_offset+1] == pedate[m_offset+1-1] ) {
            tmask[m_offset+2] = m_base->get_field_last( &tmask[1], m_offset+2-1 );
        }
    }

    tmask = get_base_fields( &tmask[0] );
    if( m_base->set_fields_as_begin_last( &fields[1], &tmask[1] ) ) {
        if( mask[0] < (int) m_dates.size()-1 ) { // No limits on last era
            Field jdn = m_base->get_jdn( &fields[1] );
            if( jdn > m_dates[mask[0]+1] ) {
                m_base->set_fields( &fields[1], m_dates[mask[0]+1]-1 );
            }
        }
        return make_era_fields( fields );
    }
    return false;
}

bool Era::set_fields_as_next_last( Field* fields, const Field* mask )
{
    if( m_offset+2 >= m_rec_size ) {
        return false;
    }
    // There is a second range only when
    // mask month == m_before_era month and mask day == f_invalid
    size_t era = mask[0];
    FieldVec epoch = m_edates[era];
    if( mask[m_offset+1] != epoch[m_offset+1-1] || mask[m_offset+2] != f_invalid  ) {
        return false;
    }
    Record rec( m_base, m_epochs[era] - 1 );
    FieldVec eend = rec.get_fieldvec();
    if( fields[m_offset+2] == eend[m_offset+2-1] ) {
        // already done
        return false;
    }
    fields[m_offset+2] = eend[m_offset+2-1];
    return true;
}

void Era::remove_fields_if_first( Field* fields ) const
{
    FieldVec mask = get_base_fields( fields );
    m_base->remove_fields_if_first( &mask[1] );
    if( make_era_fields( &mask[0] ) ) {
        copy_fields( fields, &mask[0] );
    }
}

void Era::remove_fields_if_last( Field* fields ) const
{
    FieldVec mask = get_base_fields( fields );
    m_base->remove_fields_if_last( &mask[1] );
    if( make_era_fields( &mask[0] ) ) {
        copy_fields( fields, &mask[0] );
    }
}

bool Era::balance_fields( Field* firsts, Field* lasts ) const
{
    if( firsts[0] != lasts[0] ) {
        return false;
    }
    FieldVec fs = get_base_fields( firsts );
    FieldVec ls = get_base_fields( lasts );
    bool ret = m_base->balance_fields( &fs[1], &ls[1] );
    if( ret ) {
        ret = make_era_fields( &fs[0] );
    }
    if( ret ) {
        ret = make_era_fields( &ls[0] );
    }
    if( ret ) {
        copy_fields( &firsts[0], &fs[0] );
        copy_fields( &lasts[0], &ls[0] );
    }
    return ret;
}

void Era::set_fields( Field* fields, Field jdn ) const
{
    Field block = m_dates.size() - 1;
    for( size_t i = 1 ; i < m_dates.size() ; i++ ) {
        if( jdn < m_dates[i] ) {
            block = i - 1;
            break;
        }
    }
    fields[0] = block;
    m_base->set_fields( &fields[1], jdn );
    make_era_fields( fields );
}

bool Era::set_field_first( Field* fields, size_t index ) const
{
    assert( false );
    return false;
}

bool Era::set_field_last( Field* fields, size_t index ) const
{
    assert( false );
    return false;
}

FieldVec Era::get_base_fields( const Field* mask ) const
{
    FieldVec fields( m_rec_size );
    for( size_t i = 0 ; i < m_rec_size ; i++ ) {
        fields[i] = mask[i];
        if( i == m_offset && mask[0] >= 0 && mask[0] < (int) m_epochs.size() ) {
            FieldVec epoch = m_edates[mask[0]];
            fields[i] += epoch[m_offset-1];
            Field comp = m_base->compare_minor_fields( &mask[1], &epoch[0], m_offset-1 );
            if( comp <= 0 && comp != f_invalid ) {
                --fields[m_offset];
            }
        }
    }
    return fields;
}

bool Era::make_era_fields( Field* fields ) const
{
    if( fields[0] >= 0 && fields[0] < (int) m_epochs.size() ) {
        FieldVec epoch = m_edates[fields[0]];
        fields[m_offset] -= epoch[m_offset-1];
        Field comp = m_base->compare_minor_fields( &fields[1], &epoch[0], m_offset-1 );
        if( comp <= 0 && comp != f_invalid ) {
            fields[m_offset]++;
        }
        return true;
    }
    return false;
}

// End of src/cal/calera.cpp
