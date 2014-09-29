/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calhybrid.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Hybrid variant calendar implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     23rd September 2013
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

#include "calhybrid.h"

#include "cal/calendars.h"
#include "calparse.h"
#include "calscheme.h"
#include "calrecord.h"

using namespace Cal;
using namespace std;

Hybrid::Hybrid( Calendars* cals, const string& data )
    : m_rec_size(0), m_max_child_size(0), Base()
{
    string body;
    vector<string> statements = parse_statements( peel_cbrackets( data ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string statement = get_first_word( statements[i], &body );
        if( statement == "fields" ) {
            create_fieldnames( body );
        } else if( statement == "scheme" ) {
            add_scheme( cals, body );
        }
    }
}

Hybrid::Hybrid( 
    const StringVec& fields, const std::vector<Base*>& bases, const FieldVec& dates )
    : m_fieldnames(fields), m_bases(bases), m_dates(dates),
    m_max_child_size(0), Base()
{
    m_rec_size = m_fieldnames.size() + 1;
    XRefVec xref( m_fieldnames.size() );
    for( size_t i = 0 ; i < m_bases.size() ; i++ ) {
        m_max_child_size = max( m_max_child_size, m_bases[i]->record_size() );
        for( size_t j = 0 ; j < xref.size() ; j++ ) {
            xref[j] = bases[i]->get_fieldname_index( m_fieldnames[j] );
        }
        m_xref_fields.push_back( xref );
    }
}

Hybrid::~Hybrid()
{
}

bool Hybrid::is_ok() const
{
    return m_fieldnames.size() && 
        m_bases.size() > 1 && 
        m_dates.size() == m_bases.size() - 1;
}

int Hybrid::get_fieldname_index( const string& fieldname ) const
{
    if( fieldname == "scheme" ) {
        return 0;
    }
    for( size_t i = 0 ; i < m_fieldnames.size() ; i++ ) {
        if( m_fieldnames[i] == fieldname ) {
            return i + 1;
        }
    }
    return -1;
}

string Hybrid::get_fieldname( size_t index ) const
{
    if( index == 0 ) {
        return "scheme";
    }
    if( index >= m_fieldnames.size() + 1 ) {
        return "";
    }
    return m_fieldnames[index-1];
}

Field Hybrid::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid ) {
        // We can't be sure there is not several answers
        // but we will return the first (earliest) one found.
        for( size_t i = 0 ; i < m_bases.size() ; i++ ) {
            FieldVec fs = get_xref( &fields[0], i );
            Field jdn = m_bases[i]->get_jdn( &fs[1] );
            if( jdn != f_invalid && ( i == m_bases.size() - 1 || jdn < m_dates[i] ) ) {
                return jdn;
            }
        }
        return f_invalid;
    }
    FieldVec fs = get_xref( &fields[0], fields[0] );
    return m_bases[fields[0]]->get_jdn( &fs[1] );
}

Field Hybrid::get_field_last( const Field* fields, size_t index ) const
{
    FieldVec fs(m_rec_size);
    copy_fields( &fs[0], fields );
    for( Field sch = 0 ; sch < (Field) m_bases.size() ; sch++ ) {
        Field last = m_bases[sch]->get_field_last( &fields[1], index - 1 );
        fs[index] = last;
        if( is_in_scheme( m_bases[sch]->get_jdn( &fs[0] ), sch ) ) {
            return last;
        }
    }
    return f_invalid;
}

bool Hybrid::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    Field sch = ( mask[0] == f_invalid ) ? 0 : mask[0]; 
    for( ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch] );
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
    for( Field sch = fields[0] ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch], &fields[1], m_bases[sch]->record_size() );
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
    for( Field sch = fields[0]+1 ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch] );
        FieldVec tmask = get_xref( &mask[0], sch );
        if( rec.set_fields_as_begin_first( &tmask[1] ) ) {
            Field jdn = rec.get_jdn();
            if( is_in_scheme( jdn, sch ) ) {
                set_hybrid_fields( fields, rec.get_field_ptr(), sch );
                return true;
            }
        }
    }
    return false;
}

bool Hybrid::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    Field sch = ( mask[0] == f_invalid ) ? 0 : mask[0]; 
    for( ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch] );
        FieldVec tmask = get_xref( &mask[0], sch );
        bool success = rec.set_fields_as_begin_last( &tmask[1] );
        if( success ) {
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

bool Hybrid::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    if( fields[0] == f_invalid ) {
        return false;
    }
    for( Field sch = fields[0] ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch], &fields[1], m_bases[sch]->record_size() );
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
    for( Field sch = fields[0]+1 ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch] );
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
    return false;
}

void Hybrid::remove_balanced_fields( Field* left, Field ljdn, Field* right, Field rjdn ) const
{
    Base* lbase = m_bases[left[0]];
    if( left[0] == right[0] ) {
        lbase->remove_balanced_fields( &left[1], ljdn, &right[1], ljdn );
        return;
    }
    Base* rbase = m_bases[right[0]];
    size_t size = lbase->record_size();
    if( size != rbase->record_size() ) {
        return;
    }
    FieldVec ls = get_xref( &left[0], left[0] );
    FieldVec rs = get_xref( &right[0], right[0] );
    size_t i;
    for( i = size -1 ; i > 1 ; --i ) {
        if( ls[i] == f_invalid || rs[i] == f_invalid ) {
            return; // Must be fully qualified
        }
        Field l = lbase->get_field_first( &ls[1], i-1 );
        Field r = f_invalid;
        if( l == ls[i] ) {
            r = rbase->get_field_last( &rs[1], i-1 );
        }
        if( r != rs[i] ) {
            break;
        }
    }
    for( i++ ; i < size ; i++ ) {
        left[i] = right[i] = f_invalid;
    }
}

void Hybrid::set_fields( Field* fields, Field jdn ) const
{
    Field sch = find_scheme( jdn );
    fields[0] = sch;
    Record rec( m_bases[sch], jdn );
    XRefVec xref = m_xref_fields[sch];
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        if( xref[i] >= 0 ) {
            fields[i+1] = rec.get_field( xref[i] );
        } else {
            fields[i+1] = f_invalid;
        }
    }
}

XRefVec Hybrid::get_default_xref_order( int count ) const
{
    XRefVec xref( record_size(), -1 );
    for( size_t i = 1 ; i < record_size() ; i++ ) {
        xref[i-1] = i;
    }
    xref[record_size()-1] = 0;
    return xref;
}

void Hybrid::create_fieldnames( const std::string& names )
{
    string body;
    string word = get_first_word( names + " ;", &body );
    while( word != ";" ) {
        m_fieldnames.push_back( word );
        word = get_first_word( body, &body );
    }
    m_rec_size = m_fieldnames.size() + 1;
}

void Hybrid::add_scheme( Calendars* cals, const std::string& def )
{
    string body;
    string word = get_first_word( def, &body );
    Scheme* sch = cals->get_scheme( word );
    if( sch == NULL ) return;
    Base* base = sch->get_base();
    if( base == NULL ) return;
    m_bases.push_back( base );
    m_max_child_size = max( m_max_child_size, base->record_size() );
    StringVec statements = parse_statements( peel_cbrackets( body ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        word = get_first_word( statements[i], &body );
        if( word == "begin" ) {
            Field date = str_to_field( body );
            m_dates.push_back( date );
        }
    }
    XRefVec xref( m_fieldnames.size() );
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        xref[i] = base->get_fieldname_index( m_fieldnames[i] );
    }
    m_xref_fields.push_back( xref );
}

FieldVec Hybrid::get_xref( const Field* fields, Field sch ) const
{
    FieldVec xref = m_xref_fields[sch];
    FieldVec result( m_rec_size, f_invalid );
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

void Hybrid::set_xref( Field* fields, const Field* mask, Field sch ) const
{
    FieldVec xref = m_xref_fields[sch];
    fields[0] = sch;
    for( size_t i = 1 ; i < xref.size()+1 ; i++ ) {
        if( xref[i-1] >= 0 ) {
            fields[i] = mask[xref[i-1]];
        }
    }
}


bool Hybrid::is_in_scheme( Field jdn, Field scheme ) const
{
    for( size_t i = 0 ; i < m_dates.size() ; i++ ) {
        if( jdn < m_dates[i] ) {
            return scheme == (Field) i;
        }
    }
    return scheme == m_dates.size();
}

Field Hybrid::find_scheme( Field jdn ) const
{
    for( size_t i = 0 ; i < m_dates.size() ; i++ ) {
        if( jdn < m_dates[i] ) {
            return i;
        }
    }
    return m_dates.size();
}

void Hybrid::set_hybrid_fields( Field* fields, const Field* mask, Field sch ) const
{
    fields[0] = sch;
    FieldVec xref = m_xref_fields[sch];
    for( size_t i = 0 ; i < m_bases[sch]->record_size() ; i++ ) {
        if( xref[i] < 0 ) {
            continue;
        }
        fields[i+1] = mask[xref[i]];
    }
}

// End of src/cal/calhybrid.cpp
