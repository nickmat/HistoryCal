/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calgrammar.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Grammar class to control date formatting.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th November 2013
 * Copyright:   Copyright (c) 2013 ~ 2016, Nick Matthews.
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
#include "calformatiso.h"
#include "calformattext.h"
#include "calparse.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;

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

void Grammar::set_inherit( Calendars* cals, const std::string& code )
{
    m_inherit = cals->get_grammar( code );
}

void Grammar::set_pref( const std::string& fcode )
{
    Format* fmt = get_format( fcode );
    if( fmt ) {
        m_pref_output_fcode = fcode;
        m_pref_input_fcode = fcode;
    }
}

void Grammar::add_vocab( Vocab* vocab )
{
    if( vocab ) {
        m_vocabs.push_back( vocab );
    }
}

FormatText* Grammar::create_format_text( const string& code )
{
    if( m_formats.count( code ) ) {
        // Already there
        return NULL;
    }
    FormatText* fmt = new FormatText( code, this );
    m_formats[code] = fmt;
    return fmt;
}

FormatIso* Grammar::create_format_iso( const string& code, const StringVec& rules )
{
    if( m_formats.count( code ) ) {
        // Already there
        return NULL;
    }
    FormatIso* fmt = new FormatIso( code, this, rules );
    m_formats[code] = fmt;
    return fmt;
}

bool Grammar::add_format( Format* fmt )
{
    assert( fmt != NULL );
    string code = fmt->get_code();
    if( m_formats.count( code ) ) {
        // Already there
        return false;
    }
    m_formats[code] = fmt;
    // Set preferences to the first format by default.
    if( m_pref_input_fcode.empty() ) {
        m_pref_input_fcode = code;
    }
    if( m_pref_output_fcode.empty() ) {
        m_pref_output_fcode = code;
    }
    return true;
}

void Grammar::add_alias( const string& alias, const StringVec& pairs )
{
    assert( pairs.size() % 2 == 0 ); // pairs must be even.
    if( alias == "field" ) {
        for( size_t i = 0 ; i < pairs.size() ; i += 2 ) {
            m_field_alias[pairs[i]] = pairs[i+1];
        }
    } else if( alias == "stylename" ) {
        for( size_t i = 0 ; i < pairs.size() ; i += 2 ) {
            m_num_code_alias[pairs[i]] = pairs[i+1];
        }
    } else if( alias == "unit" ) {
        for( size_t i = 0 ; i < pairs.size() ; i += 2 ) {
            string name = pairs[i+1];
            Unit unit = unit_null;
            if( name == "year" ) {
                unit = unit_year;
            } else if( name == "month" ) {
                unit = unit_month;
            } else if( name == "week" ) {
                unit = unit_week;
            } else if( name == "day" ) {
                unit = unit_day;
            } else {
                continue;
            }
            string key = make_key( pairs[i] );
            m_unit_alias[key] = unit;
        }
    }
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
    FormatMap::const_iterator it2 = m_formats.find( code );
    if( it2 != m_formats.end() ) {
        return it2->second->get_user_input_str();
    }
    if( m_inherit ) {
        return m_inherit->get_input_format( code );
    }
    return "";
}

void Grammar::get_input_formats( SchemeFormats* input, const std::string& cur_code ) const
{
    int cur = input->code.size();
    for( FormatMap::const_iterator it = m_formats.begin() ; 
        it != m_formats.end() ; it++
    ) {
        Format* fmt = it->second;
        string code = fmt->get_code();
        string order = fmt->get_user_input_str();
        if( order.empty() ) {
            continue;
        }
        bool exists = false;
        for( size_t i = 0 ; i < input->descrip.size() ; i++ ) {
            if( input->descrip[i] == order ) {
                if( code == cur_code ) {
                    input->current = i;
                }
                exists = true;
                break;
            }
        }
        if( !exists ) {
            if( code == m_pref_input_fcode ) {
                input->current = input->code.size();
            }
            input->code.push_back( code );
            input->descrip.push_back( order );
        }
    }
    if( m_inherit ) {
        m_inherit->get_input_formats( input, cur_code );
    }
}

void Grammar::get_output_formats( SchemeFormats* output, const std::string& cur_code ) const
{
    int cur = output->code.size();
    for( FormatMap::const_iterator it = m_formats.begin() ; 
        it != m_formats.end() ; it++
    ) {
        bool exists = false;
        for( size_t i = 0 ; i < output->code.size() ; i++ ) {
            if( it->first == output->code[i] ) {
                exists = true;
            }
        }
        if( exists ) {
            continue;
        }
        output->code.push_back( it->first );
        output->descrip.push_back( it->second->get_user_output_str() );
        if( it->first == cur_code ) {
            output->current = cur;
        }
        cur++;
    }
    if( m_inherit ) {
        m_inherit->get_output_formats( output, cur_code );
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

Field Grammar::find_token( Vocab** vocab, const std::string& word ) const
{
    Field field = f_invalid;
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        field = m_vocabs[i]->find( word );
        if( field != f_invalid ) {
            if( vocab ) {
                *vocab = m_vocabs[i];
            }
            return field;
        }
    }
    if( m_inherit ) {
        field = m_inherit->find_token( vocab, word );
        if( field != f_invalid ) {
            return field;
        }
    }
    if( vocab ) {
        *vocab = NULL;
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
    if( code.empty() ) {
        return NULL;
    }
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

void Grammar::remove_format( const string& fcode )
{
    m_formats.erase( fcode );
}

StringVec Grammar::get_opt_fieldnames() const
{
    if( m_inherit ) {
        StringVec ofns = m_inherit->get_opt_fieldnames();
        ofns.insert( ofns.end(), m_opt_fieldnames.begin(), m_opt_fieldnames.end() );
        return ofns;
    }
    return m_opt_fieldnames;
}

StringVec Grammar::get_rank_fieldnames() const
{
    if( m_rank_fieldnames.empty() && m_inherit ) {
        return m_inherit->get_rank_fieldnames();
    }
    return m_rank_fieldnames;
}

// End of src/cal/calgrammar.cpp file
