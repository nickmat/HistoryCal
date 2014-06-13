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


Regnal::Regnal( Calendars* cals, const std::string& code, const std::string& data )
    : m_rec_size(0), Base()
{
    string body;
    StringVec statements = parse_statements( peel_cbrackets( data ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string word = get_first_word( statements[i], &body );
        if( word == "fields" ) {
            create_fieldnames( body );
        } else if( word == "schemes" ) {
            create_schemes( cals, body );
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

void Regnal::create_default_scheme( Calendars* cals, const string& code )
{
    SHandle sch = cals->get_scheme( code );
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

void Regnal::create_schemes( Calendars* cals, const std::string& statement )
{
    StringVec eras = parse_statements( peel_cbrackets( statement ) );
    for( size_t i = 0 ; i < eras.size() ; i++ ) {
        StringVec erastr = parse_statements( peel_cbrackets( eras[i] ) );
        RegnalEra era;
        string sch_def;
        StringMap matched;
        for( size_t j = 0 ; j < erastr.size() ; j++ ) {
            string body;
            string word = get_first_word( erastr[j], &body );
            if( word == "range" ) {
                word = get_first_word( body, &body );
                era.begin = str_to_field( word );
                era.end = str_to_field( body );
            } else if( word == "scheme" ) {
                sch_def = body;
            } else if( word == "match" ) {
                map_matched_fields( matched, body );
            }
        }
        add_scheme( era, cals, sch_def, matched );
        assert( era.base != NULL );
    }
}

void Regnal::map_matched_fields( StringMap& matched, const std::string data )
{
    string body, sch_field, reg_field;
    sch_field = get_first_word( data + " ;", &body );
    while( sch_field != ";" ) {
        reg_field = get_first_word( body, &body );
        if( reg_field == ";" ) {
            break;
        }
        matched[reg_field] = sch_field;
        sch_field = get_first_word( body, &body );
    };
}

void Regnal::add_scheme(
    RegnalEra& era, Calendars* cals, const string& data, const StringMap& matched )
{
    string body;
    string word = get_first_word( data, &body );
    Scheme* sch = cals->get_scheme( word );
    if( sch ) {
        era.base = sch->get_base();
    } else {
        era.scheme = new Scheme( cals, data );
        era.base = era.scheme->get_base();
    }
    for( size_t i = 0 ; i < m_fieldnames.size() ; i++ ) {
        string fieldname = m_fieldnames[i];
        if( matched.count( m_fieldnames[i] ) > 0 ) {
            fieldname = matched.find( m_fieldnames[i] )->second;
        }
        int index = era.base->get_fieldname_index( fieldname );
        era.xref.push_back( index );
    }
    m_eras.push_back( era );
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
