/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calhybrid.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Hybrid variant calendar implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     23rd September 2013
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

#include "calhybrid.h"

#include "cal/calendars.h"
#include "calformattext.h"
#include "calparse.h"
#include "calscheme.h"
#include "calrecord.h"

#include <cassert>

using namespace Cal;
using namespace std;

Hybrid::Hybrid( const StringVec& fields, const std::vector<HybridData>& data )
    :  m_fieldnames(fields), m_data(data), m_rec_size(1+fields.size())
{
    XRefVec xref( fields.size() );
    for( size_t i = 0 ; i < data.size() ; i++ ) {
        for( size_t j = 0 ; j < xref.size() ; j++ ) {
            xref[j] = data[i].base->get_fieldname_index( fields[j] );
        }
        m_xref_fields.push_back( xref );
    }
}

Hybrid::~Hybrid()
{
    for( size_t i = 0 ; i < m_data.size() ; i++ ) {
        delete m_data[i].scheme;
    }
}

bool Hybrid::is_ok() const
{
    return
        m_fieldnames.size() && 
        m_data.size() > 1 && 
        m_data.size() == m_xref_fields.size();
}

int Hybrid::get_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "scheme" ) {
        return 0;
    }
    int offset = 1;
    for( size_t i = 0 ; i < m_fieldnames.size() ; i++ ) {
        if( m_fieldnames[i] == fieldname ) {
            return i + offset;
        }
    }
    return Base::get_fieldname_index( fieldname );
}

string Hybrid::get_fieldname( size_t index ) const
{
    if( index == 0 ) {
        return "scheme";
    }
    if( index <= m_fieldnames.size() ) {
        return m_fieldnames[index-1];
    }
    return Base::get_fieldname( index );
}

Field Hybrid::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid ) {
        // We can't be sure there is not several answers
        // but we will return the first (earliest) one found.
        for( size_t i = 0 ; i < m_data.size() ; i++ ) {
            FieldVec fs = get_xref( &fields[0], i );
            Field jdn = m_data[i].base->get_jdn( &fs[1] );
            if( jdn != f_invalid && ( i == m_data.size() - 1 || jdn < m_data[i+1].start ) ) {
                return jdn;
            }
        }
        return f_invalid;
    }
    FieldVec fs = get_xref( &fields[0], fields[0] );
    if( fs[0] == f_invalid ) {
        return f_invalid;
    }
    return m_data[fields[0]].base->get_jdn( &fs[1] );
}

Field Hybrid::get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const
{
    size_t s = find_scheme( jdn );
    return m_data[s].base->get_opt_field( &fields[1], jdn, id );
}

Field Hybrid::get_rec_field_last( const Field* fields, size_t index ) const
{
    FieldVec fs(m_rec_size);
    copy_fields( &fs[0], fields );
    for( Field sch = 0 ; sch < (Field) m_data.size() ; sch++ ) {
        Field last = m_data[sch].base->get_rec_field_last( &fields[1], index - 1 );
        fs[index] = last;
        if( is_in_scheme( m_data[sch].base->get_jdn( &fs[0] ), sch ) ) {
            return last;
        }
    }
    return f_invalid;
}

bool Hybrid::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    Field sch = ( mask[0] == f_invalid ) ? 0 : mask[0]; 
    for( ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base );
        FieldVec tmask = get_xref( &mask[0], sch );
        bool ret = rec.set_fields_as_begin_first( &tmask[1] );
        if( ret == true ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
        if( mask[0] != f_invalid ) {
            return false;
        }
    }
    return false;
}

bool Hybrid::set_fields_as_next_first( Field* fields, const Field* mask ) const
{
    if( fields[0] == f_invalid ) {
        return false;
    }
    for( Field sch = fields[0] ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base, &fields[1], m_data[sch].base->record_size() );
        FieldVec tmask = get_xref( &mask[0], sch );
        bool ret = rec.set_fields_as_next_first( &tmask[1] );
        if( ret == true ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
        if( mask[0] != f_invalid ) {
            return false;
        }
    }
    for( Field sch = fields[0]+1 ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base );
        FieldVec tmask = get_xref( &mask[0], sch );
        if( rec.set_fields_as_begin_first( &tmask[1] ) ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
            // Try next.
            while( rec.set_fields_as_next_first( &tmask[1] ) ) {
                jdn = rec.get_jdn();
                if( is_in_scheme( jdn, sch ) ) {
                    set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                    return true;
                }
            }
        }
    }
    // Finally, try setting to the next block
    Field sch = fields[0] + 1;
    if( sch < (Field) m_data.size() ) {
        Record rec( m_data[sch].base, m_data[sch].start );
        set_hybrid_fields( fields, rec.get_field_ptr(), sch );
        return true;
    }
    return false;
}

bool Hybrid::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    Field sch = ( mask[0] == f_invalid ) ? 0 : mask[0]; 
    for( ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base );
        FieldVec tmask = get_xref( &mask[0], sch );
        bool success = rec.set_fields_as_begin_last( &tmask[1] );
        if( success ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            } else if( sch < (Field) m_data.size() - 1 ) {
                rec.set_jdn( m_data[sch+1].start - 1 );
                if( rec.is_mask_valid( &tmask[1], tmask.size() - 1 ) ) {
                    set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                    return true;
                }
            }
        }
        if( mask[0] != f_invalid ) {
            return false;
        }
    }
    return false;
}

bool Hybrid::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    if( fields[0] == f_invalid ) {
        return false;
    }
    for( Field sch = fields[0] ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base, &fields[1], m_data[sch].base->record_size() );
        FieldVec tmask = get_xref( &mask[0], sch );
        bool ret = rec.set_fields_as_next_last( &tmask[1] );
        if( ret == true ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
        if( mask[0] != f_invalid ) {
            return false;
        }
    }
    for( Field sch = fields[0]+1 ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base );
        FieldVec tmask = get_xref( &mask[0], sch );
        bool ret = rec.set_fields_as_begin_last( &tmask[1] );
        if( ret == true ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
    }
    // Finally, try setting to the end of block
    Field sch = fields[0] + 1;
    if( sch < (Field) m_data.size() ) {
        Record rec( m_data[sch].base, m_data[sch].start - 1 );
        set_hybrid_fields( fields, rec.get_field_ptr(), sch );
        return true;
    }
    return false;
}

void Hybrid::remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const
{
    Base* lbase = m_data[left[0]].base;
    if( left[0] == right[0] ) {
        lbase->remove_balanced_fields( &left[1], ljdn, &right[1], ljdn );
        return;
    }
    Base* rbase = m_data[right[0]].base;
    size_t size = lbase->record_size();
    if( size != rbase->record_size() ) {
        return;
    }
    FieldVec ls = get_xref( &left[0], left[0] );
    FieldVec rs = get_xref( &right[0], right[0] );
    size_t i;
    for( i = size ; i > 1 ; --i ) {
        if( ls[i] == f_invalid || rs[i] == f_invalid ) {
            return; // Must be fully qualified
        }
        Field l = lbase->get_rec_field_first( &ls[1], i-1 );
        Field r = f_invalid;
        if( l == ls[i] ) {
            r = rbase->get_rec_field_last( &rs[1], i-1 );
        }
        if( r != rs[i] ) {
            break;
        }
    }
    for( i++ ; i < size + 1 ; i++ ) {
        left[i] = right[i] = f_invalid;
    }
}

BoolVec Hybrid::mark_balanced_fields(
    Field* left, Field ljdn, Field* right, Field rjdn, const XRefVec& rank ) const
{
    size_t test = extended_size();
    XRefVec rank1( rank.size() - 1 );
    for ( size_t i = 0, j = 0; i < rank.size(); i++ ) {
        if ( rank[i] == 0 ) {
            continue;
        }
        rank1[j++] = rank[i] - 1;
    }

    Base* lbase = m_data[left[0]].base;
    if ( left[0] == right[0] ) {
        BoolVec m = lbase->mark_balanced_fields( &left[1], ljdn, &right[1], ljdn, rank1 );
        m.insert( m.begin(), true );
        return m;
    }

    BoolVec mask( extended_size(), true );
    Base* rbase = m_data[right[0]].base;
    size_t size = lbase->record_size();
    if ( size != rbase->record_size() ) {
        return mask;
    }

    FieldVec ls = get_xref( &left[0], left[0] );
    FieldVec rs = get_xref( &right[0], right[0] );
    size_t i;
    for ( i = size; i > 1; --i ) {
        if ( ls[i] == f_invalid || rs[i] == f_invalid ) {
            return mask; // Must be fully qualified
        }
        Field l = lbase->get_rec_field_first( &ls[1], i - 1 );
        Field r = f_invalid;
        if ( l == ls[i] ) {
            r = rbase->get_rec_field_last( &rs[1], i - 1 );
        }
        if ( r != rs[i] ) {
            break;
        }
    }

    for ( i++; i < size + 1; i++ ) {
        mask[i] = false;
    }
    return mask;
}

void Hybrid::set_fields( Field* fields, Field jdn ) const
{
    Field sch = find_scheme( jdn );
    fields[0] = sch;
    Record rec( m_data[sch].base, jdn );
    XRefVec xref = m_xref_fields[sch];
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        if( xref[i] >= 0 ) {
            fields[i+1] = rec.get_field( xref[i] );
        } else {
            fields[i+1] = f_invalid;
        }
    }
}

bool Hybrid::fields_ok( const Field* fields ) const
{
    return fields[0] == f_invalid || (size_t) fields[0] < m_data.size();
}

void Hybrid::resolve_opt_input( Field* fields, size_t index ) const
{
    // Under Hybrid rules, all schemes are base on the same base calendar,
    // so they would all give the same result. Use 0 for convenience.
    m_data[0].base->resolve_opt_input( &fields[1], index - 1 );
}

FieldVec Hybrid::get_xref( const Field* fields, Field sch ) const
{
    FieldVec result( m_rec_size, f_invalid );
    if( sch >= (Field) m_xref_fields.size() || sch < 0 ) {
        return result;
    }
    FieldVec xref = m_xref_fields[sch];
    // note, field[0] may or may not match sch 
    result[0] = sch;
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        int x = xref[i];
        if( x < 0 || x+1 >= (int) m_rec_size ) {
            continue;
        }
        result[x+1] = fields[i+1];
    }
    return result;
}

bool Hybrid::is_in_scheme( Field jdn, Field scheme ) const
{
    for( size_t i = 1 ; i < m_data.size() ; i++ ) {
        if( jdn < m_data[i].start ) {
            return scheme == (Field) i - 1;
        }
    }
    return scheme == ( m_data.size() - 1 );
}

size_t Hybrid::find_scheme( Field jdn ) const
{
    for( size_t i = 1 ; i < m_data.size() ; i++ ) {
        if( jdn < m_data[i].start ) {
            return i-1;
        }
    }
    return m_data.size() - 1;
}

void Hybrid::set_hybrid_fields( Field* fields, const Field* mask, Field sch ) const
{
    fields[0] = sch;
    FieldVec xref = m_xref_fields[sch];
    for( size_t i = 0 ; i < m_data[sch].base->record_size() ; i++ ) {
        if( xref[i] < 0 ) {
            continue;
        }
        fields[i+1] = mask[xref[i]];
    }
}

// End of src/cal/calhybrid.cpp
