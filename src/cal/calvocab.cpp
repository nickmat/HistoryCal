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

#include <utf8/utf8api.h>

using namespace std;
using namespace Cal;

Vocab::Vocab( const std::string& definition )
{
    string body;
    m_code = get_first_word( definition, &body );
    StringVec statements = parse_statements( peel_cbrackets( body ) );
    for( size_t i = 0 ; i < statements.size() ; i++ ) {
        string statement = get_first_word( statements[i], &body );
        if( statement == "name" ) {
            m_name = body;
        } else if( statement == "style" ) {
            m_style = body;
        } else if( statement == "lang" ) {
            m_lang = body;
        } else if( statement == "lang" ) {
            m_lang = body;
        } else if( statement == "field" ) {
            m_field = body;
        } else if( statement == "tokens" ) {
            StringVec tokens = parse_statements( peel_cbrackets( body ) );
            for( size_t j = 0 ; j < tokens.size() ; j++ ) {
                string value = get_first_word( tokens[j], &body );
                Field field = strtol( value.c_str(), NULL, 0 );
                Token token( field, body );
                string key = Utf8api::normal( body );
                m_words[key] = token;
                m_fields[field] = token;
            }
        }
    }
}

void Vocab::get_info( Vocab_info* info ) const
{
    info->name = m_name;
    info->style = m_style;
    info->lang = m_lang;
    info->field = m_field;
    for( map<Field,Token>::const_iterator it = m_fields.begin() ; it != m_fields.end() ; it++ ) {
        Token token = it->second;
        info->tokens.push_back( token.get_word() );
    }
}

Field Vocab::find( const std::string& word ) const
{
    string key = Utf8api::normal( word );
    if( m_words.count( key ) > 0 ) {
        Token token = m_words.find( key )->second;
        return token.get_field();
    }
    return f_invalid;
}

std::string Vocab::lookup( Field field ) const
{
    if( m_fields.count( field ) > 0 ) {
        Token token = m_fields.find( field )->second;
        return token.get_word();
    }
    return "";
}

// End of src/cal/calvocab.cpp file
