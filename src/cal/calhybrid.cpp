/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calhybrid.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Hybrid variant calendar implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     23rd September 2013
 * Copyright:   Copyright (c) 2013 ~ 2020, Nick Matthews.
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
        return m_fieldnames.size();
    }
    int i = find_in_vec( fieldname, m_fieldnames );
    if ( i >= 0 ) return i;
    return Base::get_fieldname_index( fieldname );
}

string Hybrid::get_fieldname( size_t index ) const
{
    if( index == m_fieldnames.size() ) {
        return "scheme";
    }
    if( index < m_fieldnames.size() ) {
        return m_fieldnames[index];
    }
    return Base::get_fieldname( index );
}

Field Hybrid::get_jdn( const Field* fields ) const
{
    Field sch = fields[m_rec_size-1];
    if (sch == f_invalid ) {
        return f_invalid;
    }
    FieldVec fs = get_xref( fields, sch );
    return m_data[sch].base->get_jdn( &fs[0] );
}

Field Hybrid::get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const 
{
    if ( jdn == f_invalid ) {
        return f_invalid;
    }
    size_t sch = find_scheme( jdn );
    FieldVec fs = get_xref( fields, sch );
    return m_data[sch].base->get_opt_field( &fs[0], jdn, id );
}

Field Cal::Hybrid::get_rec_field_first( const Field * fields, size_t index ) const
{
    // TODO:
    return f_invalid;
}

Field Hybrid::get_rec_field_last( const Field* fields, size_t index ) const
{
    // TODO:
    return f_invalid;
}

bool Hybrid::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    Field sch = mask[m_rec_size - 1];
    if ( sch == f_invalid ) {
        sch = 0;
    }
    for( ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base );
        FieldVec tmask = get_xref( mask, sch );
        bool ret = rec.set_fields_as_begin_first( &tmask[0] );
        if( ret == true ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
        if( mask[m_rec_size - 1] != f_invalid ) {
            return false;
        }
    }
    return false;
}

bool Hybrid::set_fields_as_next_first( Field* fields, const Field* mask ) const
{
    Field scheme = fields[m_rec_size - 1];
    if ( scheme == f_invalid ) {
        return false;
    }
    for( Field sch = scheme; sch < (Field) m_data.size() ; sch++ ) {
        FieldVec tmask = get_xref( mask, sch );
        // First check if the start of the scheme block is valid for the mask.
        Record rec( m_data[sch].base, m_data[sch].start );
        if ( rec.is_mask_valid( &tmask[0], tmask.size() - 1 ) ) {
            set_hybrid_fields( fields, rec.get_field_ptr(), sch );
            return true;
        }
        // Test if this scheme shows up another block.
        rec.set_fields( fields, m_data[sch].base->record_size() );
        bool ret = rec.set_fields_as_next_first( &tmask[0] );
        if( ret == true ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
    }
    // Check for overlapping cases.
    for( Field sch = scheme + 1 ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base );
        FieldVec tmask = get_xref( mask, sch );
        if( rec.set_fields_as_begin_first( &tmask[0] ) ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
            // Try next.
            while( rec.set_fields_as_next_first( &tmask[0] ) ) {
                jdn = rec.get_jdn();
                if( is_in_scheme( jdn, sch ) ) {
                    set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                    return true;
                }
            }
        }
    }
    return false;
}

bool Hybrid::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    Field sch = mask[m_rec_size - 1];
    if ( sch == f_invalid ) {
        sch = 0;
    }
    for( ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base );
        FieldVec tmask = get_xref( mask, sch );
        bool success = rec.set_fields_as_begin_last( &tmask[0] );
        if( success ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            } else if( sch < (Field) m_data.size() - 1 ) {
                rec.set_jdn( m_data[sch+1].start - 1 );
                if( rec.is_mask_valid( &tmask[0], tmask.size() - 1 ) ) {
                    set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                    return true;
                }
            }
        }
        if( mask[m_rec_size - 1] != f_invalid ) {
            return false;
        }
    }
    return false;
}

bool Hybrid::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    Field scheme = fields[m_rec_size - 1];
    if ( scheme == f_invalid ) {
        return false;
    }
    for( Field sch = scheme ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base, fields, m_data[sch].base->record_size() );
        FieldVec tmask = get_xref( mask, sch );
        bool ret = rec.set_fields_as_next_last( &tmask[0] );
        if( ret == true ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
    }
    for( Field sch = scheme + 1 ; sch < (Field) m_data.size() ; sch++ ) {
        Record rec( m_data[sch].base );
        FieldVec tmask = get_xref( mask, sch );
        bool ret = rec.set_fields_as_begin_last( &tmask[0] );
        if( ret == true ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
    }
    return false;
}

BoolVec Hybrid::mark_balanced_fields( // <<====<<< 
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
    size_t sch_pos = m_rec_size - 1;
    const Base* lbase = m_data[left[sch_pos]].base;
    if ( left[sch_pos] == right[sch_pos] ) {
        BoolVec m = lbase->mark_balanced_fields( &left[0], ljdn, &right[0], ljdn, rank1 );
        m.insert( m.begin(), true );
        return m;
    }

    BoolVec mask( extended_size(), true );
    const Base* rbase = m_data[right[sch_pos]].base;
    size_t size = lbase->record_size();
    if ( size != rbase->record_size() ) {
        return mask;
    }

    FieldVec ls = get_xref( &left[0], left[0] );
    FieldVec rs = get_xref( &right[0], right[0] );
    size_t i;
    for ( i = size; i > 0; --i ) {
        if ( ls[i] == f_invalid || rs[i] == f_invalid ) {
            return mask; // Must be fully qualified
        }
        Field l = lbase->get_rec_field_first( &ls[0], i );
        Field r = f_invalid;
        if ( l == ls[i] ) {
            r = rbase->get_rec_field_last( &rs[0], i );
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
    fields[m_fieldnames.size()] = sch;
    Record rec( m_data[sch].base, jdn );
    XRefVec xref = m_xref_fields[sch];
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        if( xref[i] >= 0 ) {
            fields[i] = rec.get_field( xref[i] );
        } else {
            fields[i] = f_invalid;
        }
    }
}

bool Hybrid::fields_ok( const Field* fields ) const
{
    Field sch = fields[m_rec_size - 1];
    return sch == f_invalid || ( sch >= 0 && size_t( sch ) < m_data.size() );
}

FieldVec Hybrid::get_xref( const Field* fields, Field sch ) const
{
    FieldVec result( m_rec_size, f_invalid );
    if( sch >= (Field) m_xref_fields.size() || sch < 0 ) {
        return result;
    }
    FieldVec xref = m_xref_fields[sch];
    // note, field[0] may or may not match sch 
    result[m_fieldnames.size()] = sch;
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        int x = xref[i];
        if ( x < 0 || x >= int( m_fieldnames.size() ) ) {
            continue;
        }
        result[x] = fields[i];
    }
    return result;
}

bool Hybrid::is_in_scheme( Field jdn, Field scheme ) const
{
    if ( jdn == f_invalid || scheme < 0 ) {
        return false;
    }
    if ( jdn < m_data[scheme].start ) {
        return false;
    }
    if ( scheme < int(m_data.size()) - 1 && jdn >= m_data[scheme+1].start ) {
        return false;
    }
    return true;
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
    fields[m_rec_size-1] = sch;
    FieldVec xref = m_xref_fields[sch];
    for( size_t i = 0 ; i < m_data[sch].base->record_size() ; i++ ) {
        if( xref[i] < 0 ) {
            continue;
        }
        fields[i] = mask[xref[i]];
    }
}

// End of src/cal/calhybrid.cpp
