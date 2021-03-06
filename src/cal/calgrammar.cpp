/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calgrammar.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Grammar class to control date formatting.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     13th November 2013
 * Copyright:   Copyright (c) 2013 ~ 2020, Nick Matthews.
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

#include "calbase.h"
#include "cal/calendars.h"
#include "calformatiso.h"
#include "calformattext.h"
#include "calformatunit.h"
#include "calparse.h"
#include "calrecord.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;

Grammar::Grammar( const string& code, Calendars* cals )
    : m_cals( cals ), m_code( code ), m_ok( false ), m_inherit( nullptr ),
    m_sig_rank_size(0)
{
}

Grammar::~Grammar()
{
    for ( FormatMap::iterator it = m_formats.begin(); it != m_formats.end(); it++ ) {
        delete it->second;
    }
}

bool Grammar::constuct( const Base* base )
{
    if ( m_ok ) {
        return false; // Only run construct once.
    }

    if ( m_inherit ) {
        if ( m_base_fieldnames.empty() ) {
            // Use the same as original.
            m_base_fieldnames = m_inherit->m_base_fieldnames;
            if ( m_rank_fieldnames.empty() ) {
                m_rank_fieldnames = m_inherit->m_rank_fieldnames;
                m_sig_rank_size = m_inherit->m_sig_rank_size;
            }
        } else {
            // Check inherited base fieldnames are all included
            StringVec base_fns = m_inherit->m_base_fieldnames;
            for ( string iname : m_inherit->m_base_fieldnames ) {
                bool found = false;
                for ( string name : m_base_fieldnames ) {
                    if ( iname == name ) {
                        found = true;
                        break;
                    }
                }
                if ( !found ) {
                    return false;
                }
            }
        }
        StringVec opt_fields = m_opt_fieldnames;
        m_opt_fieldnames = m_inherit->m_opt_fieldnames;
        for ( string name : opt_fields ) {
            bool found = false;
            for ( string orig_name : m_inherit->m_opt_fieldnames ) {
                if ( name == orig_name ) {
                    found = true;
                    break;
                }
            }
            if ( !found ) {
                m_opt_fieldnames.push_back( name );
            }
        }
    }
    if ( base && m_base_fieldnames.empty() ) { // Anonymous grammar.
        m_base_fieldnames = base->get_base_fieldnames();
    }
    if ( m_base_fieldnames.empty() ) {
        return false;
    }

    if ( m_sig_rank_size == 0 ) {
        m_sig_rank_size = m_rank_fieldnames.size();
    }
    if ( m_sig_rank_size == 0 ) {
        m_sig_rank_size = m_base_fieldnames.size();
    }

    m_record_fieldnames = m_base_fieldnames;
    stringvec_cat( m_record_fieldnames, m_opt_fieldnames );

    // Pad out ranking field names
    StringVec rank_fields = m_rank_fieldnames;
    for ( string name : m_record_fieldnames ) {
        bool found = false;
        for( string rank_name : m_rank_fieldnames ) {
            if ( name == rank_name ) {
                found = true;
                break;
            }
        }
        if ( !found ) {
            m_rank_fieldnames.push_back( name );
        }
    }

    create_def_format();
    create_u_format();

    // Construct all contained format definitions.
    for ( auto pair : m_formats ) {
        pair.second->construct();
    }
    m_ok = true;
    return true;
}

void Cal::Grammar::create_def_format()
{
    FormatText* fmt = create_format_text( "def" );
    if ( fmt == nullptr ) {
        return;
    }
    string control;
    for ( string fieldname : m_record_fieldnames ) {
        if ( !control.empty() ) {
            control += "| ";
        }
        control += "(" + fieldname + ")";
    }
    fmt->set_control_in( control );
    fmt->set_control_out( control );
    fmt->set_style( FMT_STYLE_Hide );
}

void Cal::Grammar::create_u_format()
{
    FormatUnit* fmt = create_format_unit( "u" );
    if ( fmt == nullptr ) {
        return;
    }
    fmt->set_style( FMT_STYLE_Hide );
}

void Grammar::set_inherit( const string& code )
{
    if ( m_cals ) {
        m_inherit = m_cals->get_grammar( code );
    }
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

FormatUnit* Grammar::create_format_unit( const string& code )
{
    if( m_formats.count( code ) ) {
        // Already there
        return nullptr;
    }
    FormatUnit* fmt = new FormatUnit( code, this );
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

void Grammar::add_element( const string& elem, const ElementField& expression )
{
    m_elements[elem] = expression;
}

void Grammar::add_alias( const string& alias, const StringVec& pairs )
{
    assert( pairs.size() % 2 == 0 ); // pairs must be even.
    if( alias == "field" ) {
        for( size_t i = 0 ; i < pairs.size() ; i += 2 ) {
            m_field_alias[pairs[i]] = pairs[i+1];
        }
    } else if ( alias == "pseudo" ) {
        for ( size_t i = 0; i < pairs.size(); i += 2 ) {
            m_num_pseudo_alias[pairs[i + 1]] = pairs[i];
        }
    } else if( alias == "unit" ) {
        for( size_t i = 0 ; i < pairs.size() ; i += 2 ) {
            m_unit_alias[pairs[i]] = pairs[i+1];
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
            m_unit_type_alias[key] = unit;
        }
    }
}

string Grammar::get_field_alias( const string& fname ) const
{
    if( m_field_alias.count( fname ) > 0 ) {
        return m_field_alias.find( fname )->second;
    }
    if ( m_elements.count( fname ) > 0 ) {
        string fn = m_elements.find( fname )->second.alias;
        if ( !fn.empty() ) {
            return fn;
        }
    }
    if( m_inherit ) {
        return m_inherit->get_field_alias( fname );
    }
    return fname;
}

string Grammar::get_unit_fieldname( Field* multiple, const string& unit ) const
{
    if( m_unit_alias.count( unit ) > 0 ) {
        return m_unit_alias.find( unit )->second;
    }
    if( m_inherit ) {
        return m_inherit->get_unit_fieldname( multiple, unit );
    }
    return get_field_alias( unit );
}

string Grammar::get_unitname( const string& fieldname ) const
{
    for( StringMap::const_iterator it = m_unit_alias.begin() ; it != m_unit_alias.end() ; it++ ) {
        if( it->second == fieldname ) {
            return it->first;
        }
    }
    if( m_inherit ) {
        return m_inherit->get_unitname( fieldname );
    }
    return fieldname;
}

string Grammar::get_num_pseudo_alias( const string& fname ) const
{
    if( m_num_pseudo_alias.count( fname ) > 0 ) {
        return m_num_pseudo_alias.find( fname )->second;
    }
    if ( m_field_alias.count( fname ) > 0 ) {
        string fname_alias = m_field_alias.find( fname )->second;
        if ( m_num_pseudo_alias.count( fname_alias ) > 0 ) {
            return m_num_pseudo_alias.find( fname_alias )->second;
        }
    }
    if( m_inherit ) {
        return m_inherit->get_num_pseudo_alias( fname );
    }
    return fname;
}

string Grammar::get_element_pseudo_name( const string& fname ) const
{
    string pseudo;
    if ( m_elements.count( fname ) == 1 ) {
        pseudo = m_elements.find( fname )->second.pseudo;
    }
    if ( pseudo.empty() && m_inherit ) {
        pseudo = m_inherit->get_element_pseudo_name( fname );
    }
    return pseudo;
}

Unit Grammar::get_unit_alias( const string& str ) const
{
    string key = make_key( str );
    if( m_unit_type_alias.count( key ) > 0 ) {
        return m_unit_type_alias.find( key )->second;
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

bool Grammar::has_input_format() const
{
    return has_format( INPUT_INFO );
}

void Grammar::get_input_formats( SchemeFormatInfo* info, const std::string& cur_code ) const
{
    if( info != NULL ) {
        info->descs.clear();
        info->current = 0;
        get_format_info( info, cur_code, INPUT_INFO );
    }
}

bool Grammar::has_output_format() const
{
    return has_format( OUTPUT_INFO );
}

void Grammar::get_output_formats( SchemeFormatInfo* info, const std::string& cur_code ) const
{
    if( info != NULL ) {
        info->descs.clear();
        info->current = 0;
        get_format_info( info, cur_code, OUTPUT_INFO );
    }
}

Format* Grammar::get_format( const string& code ) const
{
    FormatMap::const_iterator it = m_formats.find( code );
    if( it == m_formats.end() ) {
        if( m_inherit &&  code != "def" && code != "u" ) {
            return m_inherit->get_format( code );
        }
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

ElementField* Grammar::get_calc_field( const string& name )
{
    if ( m_elements.count( name ) == 1 ) {
        return &m_elements.find( name )->second;
    }
    return nullptr;
}

bool Grammar::get_element(
    Field* field, const Record& record, const string& fname, const BoolVec* reveal ) const
{
    if ( m_elements.count( fname ) == 1 ) {
        string expression = m_elements.find( fname )->second.out_expression;
        if ( !expression.empty() ) {
            *field = m_cals->evaluate_field( expression, record, reveal );
            return true;
        }
    }
    if( m_inherit ) {
        return m_inherit->get_element( field, record, fname, reveal );
    }
    return false;
}

bool Grammar::set_element( Record* record, const string& fname, Field field ) const
{
    if ( m_elements.count( fname ) == 1 ) {
        string expression = m_elements.find( fname )->second.in_expression;
        if ( !expression.empty() ) {
            m_cals->evaluate_record( expression, record, fname, field );
            return true;
        }
    }
    if ( m_inherit ) {
        return m_inherit->set_element( record, fname, field );
    }
    return false;
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
            Vocab::Pseudo style = abbrev ? Vocab::pseudo_abbrev : Vocab::pseudo_full;
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

size_t Cal::Grammar::get_sig_rank_size() const
{
    if ( m_sig_rank_size == 0 ) {
        return m_base_fieldnames.size();
    }
    return m_sig_rank_size;
}

int Grammar::get_rank_field_index( const string& fieldname ) const
{
    int cnt = 0;
    for ( string name : m_rank_fieldnames ) {
        if ( name == fieldname ) {
            return cnt;
        }
        cnt++;
    }
    return -1;
}

bool Grammar::has_format( INFO type ) const
{
    for ( auto pair : m_formats ) {
        Format* fmt = pair.second;
        if ( fmt->get_style() == FMT_STYLE_Hide ) {
            continue;
        }
        string str = type == INPUT_INFO ? fmt->get_user_input_str() : fmt->get_user_output_str();
        if ( !str.empty() ) {
            return true;
        }
    }
    if ( m_inherit ) {
        return m_inherit->has_format( type );
    }
    return false;
}

void Grammar::get_format_info( SchemeFormatInfo* info, const string& cur_code, INFO type ) const
{
    // TODO: Adjust priority for inherited grammars.
    for( FormatMap::const_iterator it = m_formats.begin() ; it != m_formats.end() ; it++ ) {
        Format* fmt = it->second;
        if ( fmt->get_style() == FMT_STYLE_Hide ) {
            continue;
        }
        string str = type == INPUT_INFO ? fmt->get_user_input_str() : fmt->get_user_output_str();
        if( str.empty() ) {
            continue;
        }
        PCode pcode;
        pcode.code = fmt->get_code();
        pcode.priority = fmt->get_priority();
        bool found = false;
        for( size_t i = 0 ; i < info->descs.size() ; i++ ) {
            if( info->descs[i].desc == str ) {
                // We are already there.
                found = true;
                if( pcode.code == cur_code ) {
                    info->current = i;
                }
                bool inserted = false;
                for( size_t j = 0 ; j < info->descs[i].codes.size() ; j++ ) {
                    int p = info->descs[i].codes[j].priority;
                    if( pcode.priority > p ) {
                        // Insert it here.
                        info->descs[i].codes.insert( info->descs[i].codes.begin() + j, pcode );
                        inserted = true;
                        break;
                    }
                }
                if( !inserted ) {
                    info->descs[i].codes.push_back( pcode );
                }
                break;
            } else {
                for ( size_t j = 0; j < info->descs[i].codes.size(); j++ ) {
                    if ( pcode.code == info->descs[i].codes[j].code ) {
                        found = true;
                        break;
                    }
                }
                if ( found ) {
                    break;
                }
            }
        }
        if( !found ) {
            PDesc desc;
            desc.desc = str;
            desc.codes.push_back( pcode );
            if( pcode.code == cur_code ) {
                info->current = info->descs.size();
            }
            info->descs.push_back( desc );
        }
    }
    if( m_inherit ) {
        m_inherit->get_format_info( info, cur_code, type );
    }
}


// End of src/cal/calgrammar.cpp file
