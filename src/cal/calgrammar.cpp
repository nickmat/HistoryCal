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

#include "cal/calendars.h"
#include "calformat.h"
#include "calparse.h"
#include "calschemes.h"
#include "calvocab.h"

using namespace std;
using namespace Cal;

Grammar::Grammar( const string& code )
    : m_code(code), m_inherit(NULL)
{
}

Grammar::~Grammar()
{
    for( FormatMap::iterator it = m_formats.begin() ; it != m_formats.end() ; it++ ) {
        delete it->second;
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

void Grammar::add_vocabs( Calendars* cals, const std::string& str )
{
    string body = str;
    while( body.size() ) {
        string code = get_first_word( body, &body );
        Vocab* voc = cals->get_vocab( code );
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
    }
    string code = get_first_word( str, &str );
    if( pref ) {
        m_pref_output_fmt = code;
        m_pref_input_fmt = code;
    }
    Format* fmt = new Format(str);
    m_formats[code] = fmt;
    string order = fmt->get_order_str();
    for( StringMap::iterator it = m_input_formats.begin() ;
        it != m_input_formats.end() ; it++
    ) {
        if( it->second == order ) {
            // Already there
            if( pref ) {
                m_pref_input_fmt = it->first;
            }
            return;
        }
    }
    m_input_formats[code] = order;
}

void Grammar::set_inherit( Schemes* schemes, const std::string& code )
{
    m_inherit = schemes->get_grammar( code );
}

void Grammar::set_inherit( Calendars* cals, const std::string& code )
{
    m_inherit = cals->get_grammar( code );
}

string Grammar::get_field_alias( const string& fname ) const
{
    if( m_field_alias.count( fname ) > 0 ) {
        return m_field_alias.find( fname )->second;
    }
    if( m_inherit ) {
        return m_inherit->get_field_alias( fname );
    }
    return fname;
}

string Grammar::get_num_code_alias( const string& fname ) const
{
    if( m_num_code_alias.count( fname ) > 0 ) {
        return m_num_code_alias.find( fname )->second;
    }
    if( m_inherit ) {
        return m_inherit->get_num_code_alias( fname );
    }
    return fname;
}

Unit Grammar::get_unit_alias( const string& str ) const
{
    string key = make_key( str );
    if( m_unit_alias.count( key ) > 0 ) {
        return m_unit_alias.find( key )->second;
    }
    if( m_inherit ) {
        return m_inherit->get_unit_alias( str );
    }
    return unit_null;
}

string Grammar::get_input_format( const string& code ) const
{
    StringMap::const_iterator it = m_input_formats.find( code );
    if( it != m_input_formats.end() ) {
        return it->second;
    }
    FormatMap::const_iterator it2 = m_formats.find( code );
    if( it2 != m_formats.end() ) {
        return it2->second->get_order_str();
    }
    if( m_inherit ) {
        return m_inherit->get_input_format( code );
    }
    return "";
}

void Grammar::get_input_formats( SchemeFormats* input ) const
{
    int cur = 0;
    for( StringMap::const_iterator it = m_input_formats.begin() ; 
        it != m_input_formats.end() ; it++
    ) {
        input->code.push_back( it->first );
        input->descrip.push_back( it->second );
        if( it->first == m_pref_input_fmt ) {
            input->current = cur;
        }
        cur++;
    }
}

void Grammar::get_output_formats( SchemeFormats* output ) const
{
    int cur = 0;
    for( FormatMap::const_iterator it = m_formats.begin() ; 
        it != m_formats.end() ; it++
    ) {
        output->code.push_back( it->first );
        output->descrip.push_back( it->second->get_user_format( this ) );
        if( it->first == m_pref_output_fmt ) {
            output->current = cur;
        }
        cur++;
    }
}

Format* Grammar::get_format( const string& code ) const
{
    FormatMap::const_iterator it = m_formats.find( code );
    if( it == m_formats.end() ) {
        if( m_inherit ) {
            return m_inherit->get_format( code );
        }
        // No options left.
        return NULL;
    }
    return it->second;
}

StringVec Grammar::get_vocab_codes() const
{
    StringVec vec;
    if( m_inherit ) {
        vec = m_inherit->get_vocab_codes();
    }
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        vec.push_back( m_vocabs[i]->get_code() );
    }
    return vec;
}

StringVec Grammar::get_vocab_names() const
{
    StringVec vec;
    if( m_inherit ) {
        vec = m_inherit->get_vocab_names();
    }
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        vec.push_back( m_vocabs[i]->get_name() );
    }
    return vec;
}

Field Grammar::find_token( const std::string& word ) const
{
    Field field = f_invalid;
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        field = m_vocabs[i]->find( word );
        if( field != f_invalid ) {
            return field;
        }
    }
    if( m_inherit ) {
        field = m_inherit->find_token( word );
        if( field != f_invalid ) {
            return field;
        }
    }
    return f_invalid;
}

string Grammar::lookup_token( Field field, const std::string& vcode, bool abbrev ) const
{
    string word;
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        if( m_vocabs[i]->get_code() == vcode ) {
            Vocab::Style style = abbrev ? Vocab::style_abbrev : Vocab::style_full;
            word = m_vocabs[i]->lookup( field, style );
            if( word != "" ) {
                return word;
            }
        }
    }
    if( m_inherit ) {
        word = m_inherit->lookup_token( field, vcode, abbrev );
    }
    return word;
}

Vocab* Grammar::find_vocab( const std::string& code ) const
{
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        if( m_vocabs[i]->get_code() == code ) {
            return m_vocabs[i];
        }
    }
    if( m_inherit ) {
        return m_inherit->find_vocab( code );
    }
    return NULL;
}

// End of src/cal/calgrammar.cpp file
