/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calmulti.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Base Multi variant calendar implimentation.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     2nd October 2013
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

#include "calmulti.h"

#include "calparse.h"
#include "calscheme.h"
#include "calschemes.h"
#include "calrecord.h"

#include <cassert>

using namespace Cal;
using namespace std;

Multi::Multi( Schemes* schs, const string& data )
    : m_rec_size(0), Base()
{
    string body = data;
    do {
        string word = get_first_word( body, &body );
        Scheme* sch = schs->get_scheme( word );
        assert( sch != NULL );
        Base* base = sch->get_base();
        assert( base != NULL );
        m_bases.push_back( base );
        m_sname.push_back( word ); // TODO: check for duplicate schemes
        m_rec_size += base->record_size();
    } while( body.size() );
    assert( m_rec_size > 0 );
}

Multi::~Multi()
{
}

int Multi::get_fieldname_index( const string& fieldname ) const
{
    string endname;
    string schname = get_first_word( fieldname, &endname, '.' );
    if( endname.empty() ) {
        return -1;
    } 
    size_t offset = 0;
    for( size_t i = 0 ; i < m_bases.size() ; i++ ) {
        if( schname == m_sname[i] ) {
            int index = m_bases[i]->get_fieldname_index( endname );
            if( index >= 0 ) {
                return index + offset;
            }
            return -1; // Might as well give up.
        }
        offset += m_bases[i]->record_size();
    }
    return -1;
}

string Multi::get_fieldname( size_t index ) const
{
    for( size_t i = 0 ; i < m_bases.size() ; i++ ) {
        string sname = m_sname[i];
        size_t size = m_bases[i]->record_size();
        if( index < size ) {
            return sname + "." + m_bases[i]->get_fieldname( index );
        }
        index -= size;
    }
    return "";
}

Field Multi::get_jdn( const Field* fields ) const
{
    size_t offset = 0;
    for( size_t i = 0 ; i < m_bases.size() ; i++ ) {
        Field jdn = m_bases[i]->get_jdn( &fields[offset] );
        if( jdn != f_invalid ) {
            return jdn;
        }
        offset += m_bases[i]->record_size();
    }
    return f_invalid;
}

bool Multi::check_usable( const Field* fields ) const
{
    // We only need to check the first scheme.
    return m_bases[0]->check_usable( fields );
}

bool Multi::set_fields_as_begin_first( Field* fields, const Field* mask )
{
    bool ret = m_bases[0]->set_fields_as_begin_first( fields, mask );
    if( ret ) {
        fill_fields( fields );
    }
    return ret;
}

bool Multi::set_fields_as_next_first( Field* fields, const Field* mask )
{
    bool ret = m_bases[0]->set_fields_as_next_first( fields, mask );
    if( ret ) {
        fill_fields( fields );
    }
    return ret;
}

bool Multi::set_fields_as_begin_last( Field* fields, const Field* mask )
{
    bool ret = m_bases[0]->set_fields_as_begin_last( fields, mask );
    if( ret ) {
        fill_fields( fields );
    }
    return ret;
}

bool Multi::set_fields_as_next_last( Field* fields, const Field* mask )
{
    bool ret = m_bases[0]->set_fields_as_next_last( fields, mask );
    if( ret ) {
        fill_fields( fields );
    }
    return ret;
}

void Multi::set_fields( Field* fields, Field jdn ) const
{
    size_t offset = 0;
    for( size_t i = 0 ; i < m_bases.size() ; i++ ) {
        m_bases[i]->set_fields( &fields[offset], jdn );
        offset += m_bases[i]->record_size();
    }
}

void Multi::fill_fields( Field* fields ) const
{
    Field jdn = m_bases[0]->get_jdn( fields );
    size_t offset = m_bases[0]->record_size();
    for( size_t i = 1 ; i < m_bases.size() ; i++ ) {
        m_bases[i]->set_fields( &fields[offset], jdn );
        offset += m_bases[i]->record_size();
    }
}

// End of src/cal/calmulti.cpp
