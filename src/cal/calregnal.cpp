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

#include "calparse.h"
#include "calscheme.h"
#include "calschemes.h"

#include <cassert>

using namespace Cal;
using namespace std;

Regnal::Regnal( Schemes* schs, const std::string& code, const std::string& data )
    : m_rec_size(0), Base()
{
    StringVec statements = parse_statements( peel_cbrackets( data ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string body;
        string word = get_first_word( statements[i], &body );
        if( word == "fields" ) {
            create_fieldnames( body );
        } else if( word == "default" ) {
            create_default_scheme( schs, body );
        } else if( word == "schemes" ) {
            create_schemes( schs, body );
        }
    }
    assert( m_eras.size() > 1 );
}

Regnal::~Regnal()
{
    for( size_t i = 1 ; i < m_eras.size() ; i++ ) {
        delete m_eras[i].scheme;
    }
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

bool Regnal::set_fields_as_begin_first( Field* fields, const Field* mask )
{
    Field e = mask[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        return false;
    }
    FieldVec m = get_base_fields( mask );
    Base* base = m_eras[e].base;
    FieldVec f( base->record_size(), f_invalid );
    if( base->set_fields_as_begin_first( &f[0], &m[0] ) ) {
        return make_regnal_fields( fields, e, &f[0] );
    }
    return false;
}

bool Regnal::set_fields_as_next_first( Field* fields, const Field* mask )
{
    return false;
}

bool Regnal::set_fields_as_begin_last( Field* fields, const Field* mask )
{
    Field e = mask[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        return false;
    }
    FieldVec m = get_base_fields( mask );
    Base* base = m_eras[e].base;
    FieldVec f( base->record_size(), f_invalid );
    if( base->set_fields_as_begin_last( &f[0], &m[0] ) ) {
        return make_regnal_fields( fields, e, &f[0] );
    }
    return false;
}

bool Regnal::set_fields_as_next_last( Field* fields, const Field* mask )
{
    return false;
}

void Regnal::set_fields( Field* fields, Field jdn ) const
{
    Field e = 0;
    for( size_t i = 1 ; i < m_eras.size() ; i++ ) {
        if( jdn >= m_eras[i].begin && jdn <= m_eras[i].end ) {
            e = i;
            break;
        }
    }
    FieldVec f( m_eras[e].base->record_size(), f_invalid );
    m_eras[e].base->set_fields( &f[0], jdn );
    make_regnal_fields( fields, e, &f[0] );
}


void Regnal::create_fieldnames( const string& names )
{
    string body;
    string word = get_first_word( names + " ;", &body );
    while( word != ";" ) {
        m_fieldnames.push_back( word );
        word = get_first_word( body, &body );
    }
    m_rec_size = m_fieldnames.size() + 1;
}

void Regnal::create_default_scheme( Schemes* schs, const string& code )
{
    Scheme* sch = schs->get_scheme( code );
    if( sch ) {
        RegnalEra era;
        era.base = sch->get_base();
        for( size_t i = 0 ; i < m_fieldnames.size() ; i++ ) {
            int index = era.base->get_fieldname_index( m_fieldnames[i] );
            era.xref.push_back( index );
        }
        m_eras.push_back(era);
    }
}

void Regnal::create_schemes( Schemes* schs, const std::string& statement )
{
    StringVec eras = parse_statements( peel_cbrackets( statement ) );
    for( size_t i = 0 ; i < eras.size() ; i++ ) {
        StringVec erastr = parse_statements( peel_cbrackets( eras[i] ) );
        RegnalEra era;
        for( size_t j = 0 ; j < erastr.size() ; j++ ) {
            string body;
            string word = get_first_word( erastr[j], &body );
            if( word == "range" ) {
                word = get_first_word( body, &body );
                era.begin = str_to_field( word );
                era.end = str_to_field( body );
            } else if( word == "scheme" ) {
                add_scheme( &era, schs, body );
            }
        }
        assert( era.base != NULL );
        m_eras.push_back(era);
    }
}

void Regnal::add_scheme( RegnalEra* era, Schemes* schs, const string& data )
{
    string body;
    string word = get_first_word( data, &body );
    Scheme* sch = schs->get_scheme( word );
    if( sch ) {
        era->base = sch->get_base();
    } else {
        era->scheme = new Scheme( schs, data );
        era->base = era->scheme->get_base();
    }
    for( size_t i = 0 ; i < m_fieldnames.size() ; i++ ) {
        int index = era->base->get_fieldname_index( m_fieldnames[i] );
        era->xref.push_back( index );
    }
}

FieldVec Regnal::get_base_fields( const Field* fields ) const
{
    Field e = fields[0];
    if( e < 0 || e >= (int) m_eras.size() ) {
        // Return empty vector to show error
        FieldVec fs;
        return fs;
    }
    size_t fs_size = m_eras[e].base->record_size();
    FieldVec fs( fs_size, f_invalid );
    for( size_t i = 1 ; i < m_rec_size ; i++ ) {
        int index = m_eras[e].xref[i-1];
        if( index >= 0 && index < (int) fs_size ) {
            fs[index] = fields[i];
        }
    }
    return fs;
}

bool Regnal::make_regnal_fields( Field* fields, Field era, const Field* mask ) const
{
    fields[0] = era;
    for( size_t i = 0 ; i < m_eras[era].base->record_size() ; i++ ) {
        fields[i+1] = mask[m_eras[era].xref[i]];
    }
    return true;
}

// End of src/cal/calregnal.cpp
