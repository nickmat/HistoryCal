/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calvocab.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Vocab class to handle date name part tokens.
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

#include "calvocab.h"

#include "calparse.h"

using namespace std;
using namespace Cal;

#if 0
Vocab::Vocab( const std::string& definition )
{
    string body;
    string word = get_next_phrase( definition, &body );
    m_code = word;
    StringVec statements = parse_statements( peel_cbrackets( body ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string statement = get_next_phrase( statements[i], &body );
        if( statement == "name" ) {
            m_name = body;
        } else if( statement == "lang" ) {
            m_lang = body;
        } else if( statement == "lang" ) {
            m_lang = body;
        } else if( statement == "style-name" ) {
            m_full_name = get_next_phrase( body, &body );
            m_abbrev_name = body;
        } else if( statement == "tokens" ) {
            StringVec tokens = parse_statements( peel_cbrackets( body ) );
            for( size_t j = 0 ; j < tokens.size() ; j++ ) {
                string word = get_next_phrase( tokens[j], &body );
                Field field = str_to_field( word );
                word = get_next_phrase( body, &body );
                string abbrev = get_next_phrase( body, &body );
                Token token( field, word, abbrev );
                string key = make_key( word );
                m_words[key] = token;
                if( abbrev.size() ) {
                    key = make_key( abbrev );
                    m_words[key] = token;
                }
                m_fields[field] = token;
            }
        }
    }
}
#endif

Vocab::Vocab( const std::string& code ) : m_code(code)
{
}

void Vocab::add_token( Field value, const string& name, const string& abbrev )
{
    if( name.empty() ) {
        return;
    }
    Token token( value, name, abbrev );
    string key = make_key( name );
    m_words[key] = token;
    if( abbrev.size() && abbrev != name ) {
        key = make_key( abbrev );
        m_words[key] = token;
    }
    m_fields[value] = token;
}

string Vocab::get_style_name( Style style ) const
{
    return style == style_full ? m_full_name : m_abbrev_name;
}

void Vocab::get_info( Vocab_info* info ) const
{
    if( info ) {
        info->name = m_name;
        info->lang = m_lang;
        info->style_full_name = m_full_name;
        info->style_abbrev_name = m_abbrev_name;
        map<Field,Token>::const_iterator it;
        for( it = m_fields.begin() ; it != m_fields.end() ; it++ ) {
            Token token = it->second;
            info->words.push_back( token.get_word() );
            string abbrev = token.get_abbrev();
            if( abbrev.empty() ) {
                info->abbrevs.push_back( token.get_word() );
            } else {
                info->abbrevs.push_back( token.get_abbrev() );
            }
        }
    }
}

Field Vocab::find( const string& word ) const
{
    string key = make_key( word );
    if( m_words.count( key ) > 0 ) {
        Token token = m_words.find( key )->second;
        return token.get_field();
    }
    return f_invalid;
}

string Vocab::lookup( Field field, Style style ) const
{
    string result;
    if( m_fields.count( field ) > 0 ) {
        Token token = m_fields.find( field )->second;
        if( style == style_abbrev ) {
            result = token.get_abbrev();
        }
        if( result.empty() ) {
            result = token.get_word();
        }
    }
    return result;
}

// End of src/cal/calvocab.cpp file
