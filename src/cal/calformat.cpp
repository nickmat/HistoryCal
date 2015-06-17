/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/cal/calformat.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Format class to control the formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     15th November 2013
 * Copyright:   Copyright (c) 2013 - 2015, Nick Matthews.
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

#include "calformat.h"

#include "calgrammar.h"
#include "calparse.h"
#include "caltext.h"
#include "calvocab.h"

#include <cassert>

using namespace Cal;
using std::string;


Format::Format( const std::string& code, Grammar* gmr )
: m_code(code), m_owner(gmr), m_separators(":,"),
m_usefor_output(true), m_usefor_input(true), m_strict_input(false)
{
}

Format::~Format()
{
}

void Format::set_format( const std::string& format, Use usefor )
{
    assert( m_owner );
    assert( m_format.empty() );
    set_usefor( usefor );
    m_format = format;
    enum State { dooutput, dofname, dodname, dovocab, doabbrev };
    State state = dooutput;
    string fieldout, fname, dname, vocab, abbrev;
    for( string::const_iterator it = m_format.begin() ; it != m_format.end() ; it++ ) {
        if( state == dooutput ) {
            if( *it == '|' ) {
                m_output_str += fieldout;
                fieldout.clear();
            } else if( *it == '(' ) {
                state = dofname;
            } else {
                fieldout += *it;
                if( m_strict_input ) {
                    m_input_str += *it;
                }
            }
            continue;
        }
        if( *it == ')' ) {
            if( !m_strict_input && m_input_str.size() ) {
                m_input_str += " ";
            }
            m_input_str += fname;
            InputFieldType type = IFT_number;
            string fieldname = m_owner->get_field_alias( fname );
            string foname = m_owner->get_num_code_alias( fname );
            Vocab* voc = m_owner->find_vocab( vocab );
            if( voc ) {
                type = IFT_vocab;
                if( abbrev == "a" ) {
                    foname = voc->get_style_name( Vocab::style_abbrev );
                } else {
                    foname = voc->get_style_name( Vocab::style_full );
                }
            } else if( vocab.size() ) {
                // Look for universal number formatting.
                char ch = *vocab.begin();
                if( ch == '!' ) {
                    if( vocab == "!os" ) {
                        StringStyle ss = ( abbrev == "u" ) ? SS_uppercase : SS_undefined;
                        foname = get_ordinal_suffix_style( ss );
                    } else if( vocab == "!rn" ) {
                        StringStyle ss = ( abbrev == "l" ) ? SS_lowercase : SS_undefined;
                        foname += get_roman_numerals_style( ss );
                    } else if( vocab == "!lp" ) {
                        foname = get_left_pad_style( foname, abbrev );
                    }
                } else if( ch == '+' ) {
                    if( vocab == "+os" ) {
                        StringStyle ss = ( abbrev == "u" ) ? SS_uppercase : SS_undefined;
                        foname += get_ordinal_suffix_style( ss );
                    }
                }
            }
            fieldout += foname;

            if( dname.size() ) {
                fieldout += "/" + m_owner->get_num_code_alias( dname );

                m_input_fields.push_back( fieldname );
                m_vocabs.push_back( NULL );
                m_types.push_back( IFT_dual1 );

                fieldname = m_owner->get_field_alias( dname );
                type = IFT_dual2;
            }
            m_input_fields.push_back( fieldname );
            m_vocabs.push_back( voc );
            m_types.push_back( type );

            fname.clear();
            dname.clear();
            vocab.clear();
            abbrev.clear();
            state = dooutput;
        } else if( state == dofname && *it == '/' ) {
            state = dodname;
        } else if( state == dofname && *it == ':' ) {
            state = dovocab;
        } else if( state == dovocab && *it == '.' ) {
            state = doabbrev;
        } else {
            switch( state )
            {
            case dofname:  fname += *it;  break;
            case dodname:  dname += *it;  break;
            case dovocab:  vocab += *it;  break;
            case doabbrev: abbrev += *it; break;
            default: break;
            }
        }
    }
    m_output_str += fieldout;

    if( !m_usefor_input ) {
        m_input_str.clear();
    }
    if( !m_usefor_output ) {
        m_output_str.clear();
    }
}

void Format::set_usefor( Use usefor )
{
    switch( usefor )
    {
    case Use_inout:
        m_usefor_output = true;
        m_usefor_input = true;
        m_strict_input = false;
        break;
    case Use_output:
        m_usefor_output = true;
        m_usefor_input = false;
        m_strict_input = false;
        break;
    case Use_strict:
        m_usefor_output = true;
        m_usefor_input = true;
        m_strict_input = true;
        break;
    }
}

string Format::get_input_field( Vocab* vocab ) const
{
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        if( vocab == m_vocabs[i] ) {
            return m_input_fields[i];
        }
    }
    return "";
}

string Format::get_1st_input_field( InputFieldType type ) const
{
    for( size_t i = 0 ; i < m_types.size() ; i++ ) {
        if( type == m_types[i] ) {
            return m_input_fields[i];
        }
    }
    return "";
}

StringVec Format::get_rank_fieldnames() const
{
    if( m_rank_fieldnames.empty() ) {
        return m_owner->get_rank_fieldnames();
    }
    return m_rank_fieldnames;
}

bool Format::is_tier1( const std::string& fieldname ) const
{
    for( size_t i = 0 ; i < m_input_fields.size() ; i++ ) {
        if( fieldname == m_input_fields[i] ) {
            return true;
        }
    }
    return false;
}

// End of src/cal/calformat.cpp file
