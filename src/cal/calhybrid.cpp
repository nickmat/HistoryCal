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

#include "calparse.h"
#include "calscheme.h"
#include "calschemes.h"
#include "calrecord.h"

using namespace Cal;
using namespace std;

Hybrid::Hybrid( Schemes* schs, const string& data )
    : m_rec_size(0), m_max_child_size(0), Base()
{
    string body;
    vector<string> statements = parse_statements( peel_cbrackets( data ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string statement = get_first_word( statements[i], &body );
        if( statement == "fields" ) {
            create_fieldnames( body );
        } else if( statement == "scheme" ) {
            add_scheme( schs, body );
        }
    }
}

Hybrid::~Hybrid()
{
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
            FieldVec fs = get_xref( &fields[1], i );
            Field jdn = m_bases[i]->get_jdn( &fs[0] );
            if( jdn != f_invalid && ( i == m_bases.size() - 1 || jdn < m_dates[i] ) ) {
                return jdn;
            }
        }
        return f_invalid;
    }
    FieldVec fs = get_xref( &fields[1], fields[0] );
    return m_bases[fields[0]]->get_jdn( &fs[0] );
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

bool Hybrid::set_fields_as_begin_first( Field* fields, const Field* mask )
{
    Field sch = ( mask[0] == f_invalid ) ? 0 : mask[0]; 
    for( ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch] );
        FieldVec tmask = get_xref( &mask[1], sch );
        bool ret = rec.set_fields_as_begin_first( &tmask[0] );
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

bool Hybrid::set_fields_as_next_first( Field* fields, const Field* mask )
{
    for( Field sch = fields[0] ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch], &fields[1], m_bases[sch]->record_size() );
        FieldVec tmask = get_xref( &mask[1], sch );
        bool ret = rec.set_fields_as_next_first( &tmask[0] );
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
        FieldVec tmask = get_xref( &mask[1], sch );
        bool ret = rec.set_fields_as_begin_first( &tmask[0] );
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

bool Hybrid::set_fields_as_begin_last( Field* fields, const Field* mask )
{
    Field sch = ( mask[0] == f_invalid ) ? 0 : mask[0]; 
    for( ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch] );
        FieldVec tmask = get_xref( &mask[1], sch );
        bool ret = rec.set_fields_as_begin_last( &tmask[0] );
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

bool Hybrid::set_fields_as_next_last( Field* fields, const Field* mask )
{
    for( Field sch = fields[0] ; sch < (Field) m_bases.size() ; sch++ ) {
        Record rec( m_bases[sch], &fields[1], m_bases[sch]->record_size() );
        FieldVec tmask = get_xref( &mask[1], sch );
        bool ret = rec.set_fields_as_next_last( &tmask[0] );
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
        FieldVec tmask = get_xref( &mask[1], sch );
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

void Hybrid::remove_fields_if_first( Field* fields ) const
{
    Field sch = fields[0];
    if( sch == f_invalid ) {
        return;
    }
    FieldVec mask = get_xref( &fields[1], sch );
    m_bases[sch]->remove_fields_if_first( &mask[0] );
    set_hybrid_fields( &fields[0], &mask[0], sch );
}

void Hybrid::remove_fields_if_last( Field* fields ) const
{
    Field sch = fields[0];
    if( sch == f_invalid ) {
        return;
    }
    FieldVec mask = get_xref( &fields[1], sch );
    m_bases[sch]->remove_fields_if_last( &mask[0] );
    set_hybrid_fields( &fields[0], &mask[0], sch );
}

bool Hybrid::balance_fields( Field* firsts, Field* lasts ) const
{
    Field sch = firsts[0];
    if( sch == f_invalid || sch != lasts[0] ) {
        return Base::balance_fields( firsts, lasts );
    }
    FieldVec fmask = get_xref( &firsts[1], sch );
    FieldVec lmask = get_xref( &lasts[1], sch );
    bool ret = m_bases[sch]->balance_fields( &fmask[0], &lmask[0] );
    if( ret ) {
        set_hybrid_fields( &firsts[0], &fmask[0], sch );
        set_hybrid_fields( &lasts[0], &lmask[0], sch );
    }
    return ret;
}

bool Hybrid::set_field_first( Field* fields, size_t index ) const
{
    Field sch = fields[0];
    if( sch == f_invalid ) {
        return false;
    }
    FieldVec mask = get_xref( &fields[1], sch );
    bool ret = m_bases[sch]->set_field_first( &mask[0], index - 1 );
    if( ret ) {
        set_hybrid_fields( &fields[0], &mask[0], sch );
    }
    return ret;
}

bool Hybrid::set_field_last( Field* fields, size_t index ) const
{
    Field sch = fields[0];
    if( sch == f_invalid ) {
        return false;
    }
    FieldVec mask = get_xref( &fields[1], sch );
    bool ret = m_bases[sch]->set_field_last( &mask[0], index - 1 );
    if( ret ) {
        set_hybrid_fields( &fields[0], &mask[0], sch );
    }
    return ret;
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

void Hybrid::add_scheme( Schemes* schs, const std::string& def )
{
    string body;
    string word = get_first_word( def, &body );
    Scheme* sch = schs->get_scheme( word );
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
        } else if( word == "match" ) {
            add_match( base, body );
        }
    }
    if( m_bases.size() != m_xref_fields.size() ) {
        // Add a default xref to indicate a 1 to 1 match 
        XRefVec xref( m_rec_size - 1 );
        for( size_t i = 0 ; i < xref.size() ; i++ ) {
            xref[i] = i;
        }
        m_xref_fields.push_back( xref );
    }
}

void Hybrid::add_match( Base* base, const std::string& str )
{
    XRefVec xref( m_rec_size - 1, -1 );
    string match;
    StringVec statements = parse_statements( peel_cbrackets( str ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string word = get_first_word( statements[i], &match );
        int hybrid_field = get_fieldname_index( word );
        int match_field = base->get_fieldname_index( match );
        xref[hybrid_field-1] = match_field;
    }
    m_xref_fields.push_back( xref );
}

FieldVec Hybrid::get_xref( const Field* fields, Field sch ) const
{
    FieldVec xref = m_xref_fields[sch];
    FieldVec result(m_rec_size, f_invalid);
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        if( xref[i] < 0 ) {
            continue;
        }
        result[xref[i]] = fields[i];
    }
    return result;
}

void Hybrid::set_xref( Field* fields, const Field* mask, Field sch ) const
{
    FieldVec xref = m_xref_fields[sch];
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        if( xref[i] >= 0 ) {
            fields[i] = mask[xref[i]];
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
    for( size_t i = 0 ; i < xref.size() ; i++ ) {
        if( xref[i] < 0 ) {
            continue;
        }
        fields[i+1] = mask[xref[i]];
    }
}

// End of src/cal/calhybrid.cpp
