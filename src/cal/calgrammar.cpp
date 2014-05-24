/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calgrammar.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Grammar class to control date formatting.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th November 2013
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

#include "calgrammar.h"

#include "calformat.h"
#include "calparse.h"
#include "calschemes.h"
#include "calvocab.h"

using namespace std;
using namespace Cal;

Grammar::Grammar( const string& code )
    : m_code(code), m_pref_order(-1), m_pref_format(-1)
{
}

Grammar::~Grammar()
{
    for( size_t i = 0 ; i < m_formats.size() ; i++ ) {
        delete m_formats[i];
    }
}

void Grammar::add_alias( const std::string& alias_def )
{
    string body;
    string alias = get_first_word( alias_def, &body );
    vector<string> statements = parse_statements( peel_cbrackets( body ) );
    if( alias == "field" ) {
        for( size_t i = 0 ; i < statements.size() ; i++ ) {
            alias = get_first_word( statements[i], &body );
            m_field_alias[alias] = body;
        }
    } else if( alias == "format-number-code" ) {
        for( size_t i = 0 ; i < statements.size() ; i++ ) {
            alias = get_first_word( statements[i], &body );
            m_num_code_alias[alias] = body;
        }
    } else if( alias == "unit" ) {
        for( size_t i = 0 ; i < statements.size() ; i++ ) {
            alias = get_first_word( statements[i], &body );
            Unit unit = unit_null;
            if( body == "year" ) {
                unit = unit_year;
            } else if( body == "month" ) {
                unit = unit_month;
            } else if( body == "week" ) {
                unit = unit_week;
            } else if( body == "day" ) {
                unit = unit_day;
            } else {
                continue;
            }
            string key = make_key( alias );
            m_unit_alias[key] = unit;
        }
    }
}

void Grammar::add_vocabs( Schemes* schemes, const std::string& str )
{
    string body = str;
    while( body.size() ) {
        string code = get_first_word( body, &body );
        Vocab* voc = schemes->get_vocab( code );
        if( voc != NULL ) {
            m_vocabs.push_back( voc );
        }
    };
}

void Grammar::add_format( const std::string& format )
{
    string str = format;
    bool pref = false;
    if( format.compare( 0, 5, "pref " ) == 0 ) {
        get_first_word( format, &str );
        pref = true;
        m_pref_format = m_formats.size();
    }
    Format* fmt = new Format(str);
    m_formats.push_back( fmt );
    string order = fmt->get_order_str();
    for( size_t i = 0 ; i < m_orders.size() ; i++ ) {
        if( order == m_orders[i] ) {
            // Already there
            if( pref ) {
                m_pref_order = i;
            }
            return;
        }
    }
    if( pref ) {
        m_pref_order = m_orders.size();
    }
    m_orders.push_back( order );
}

string Grammar::get_field_alias( const string& fname ) const
{
    if( m_field_alias.count( fname ) > 0 ) {
        return m_field_alias.find( fname )->second;
    }
    return fname;
}

string Grammar::get_num_code_alias( const string& fname ) const
{
    if( m_num_code_alias.count( fname ) > 0 ) {
        return m_num_code_alias.find( fname )->second;
    }
    return fname;
}

Unit Grammar::get_unit_alias( const string& str ) const
{
    string key = make_key( str );
    if( m_unit_alias.count( key ) > 0 ) {
        return m_unit_alias.find( key )->second;
    }
    return unit_null;
}

StringVec Grammar::get_formats() const
{
    StringVec vec;
    for( size_t i = 0 ; i < m_formats.size() ; i++ ) {
        vec.push_back( m_formats[i]->get_format() );
    }
    return vec;
}

string Grammar::format( size_t index ) const
{
    return m_formats[index]->get_format();
}

StringVec Grammar::get_vocab_codes() const
{
    StringVec vec;
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        vec.push_back( m_vocabs[i]->get_code() );
    }
    return vec;
}

StringVec Grammar::get_vocab_names() const
{
    StringVec vec;
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        vec.push_back( m_vocabs[i]->get_name() );
    }
    return vec;
}

Field Grammar::find_token( const std::string& word ) const
{
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        Field field = m_vocabs[i]->find( word );
        if( field != f_invalid ) {
            return field;
        }
    }
    return f_invalid;
}

string Grammar::lookup_token( Field field, const std::string& vcode, bool abbrev ) const
{
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        if( m_vocabs[i]->get_code() == vcode ) {
            Vocab::Style style = abbrev ? Vocab::style_abbrev : Vocab::style_full;
            string word = m_vocabs[i]->lookup( field, style );
            if( word != "" ) {
                return word;
            }
        }
    }
    return "";
}

Vocab* Grammar::find_vocab( const std::string& code ) const
{
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        if( m_vocabs[i]->get_code() == code ) {
            return m_vocabs[i];
        }
    }
    return NULL;
}

// End of src/cal/calgrammar.cpp file
