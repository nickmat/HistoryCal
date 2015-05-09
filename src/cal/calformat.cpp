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

using namespace std;
using namespace Cal;


Format::Format( const std::string& code, Grammar* gmr )
    : m_code(code), m_owner(gmr)
{
}

Format::~Format()
{
}

void Format::set_format( const std::string& format, Use usefor )
{
    m_format = format;
    string fieldname, fieldout, fname, dname, vocab, abbrev;
    enum State { dooutput, dofname, dodname, dovocab, doabbrev };
    State state = dooutput;
    for( string::const_iterator it = m_format.begin() ; it != m_format.end() ; it++ ) {
        switch( state )
        {
        case dooutput:
            if( *it == '|' ) {
                if( usefor != Use_input ) {
                    m_output_str += fieldout;
                }
                fieldout.clear();
            } else if( *it == '(' ) {
                state = dofname;
            } else {
                fieldout += *it;
            }
            break;
        case dofname:
        case dodname:
        case dovocab:
        case doabbrev:
            if( *it == ')' ) {
                if( usefor != Use_output ) {
                    if( m_input_str.size() ) {
                        m_input_str += " ";
                    }
                    m_input_str += fname;
                }
                fieldname = fname;
                Vocab* voc = NULL;
                InputFieldType type = IFT_number;
                if( m_owner ) {
                    fieldname = m_owner->get_field_alias( fname );
                    if( vocab.size() ) {
                        voc = m_owner->find_vocab( vocab );
                        if( voc ) {
                            type = IFT_vocab;
                            if( abbrev == "a" ) {
                                fname = voc->get_style_name( Vocab::style_abbrev );
                            } else {
                                fname = voc->get_style_name( Vocab::style_full );
                            }
                        }
                    } else if( abbrev == "os" ) {
                        fname = get_ordinal_suffix_style();
                    } else {
                        fname = m_owner->get_num_code_alias( fname );
                    }
                }
                fieldout += fname;
                m_output_fields.push_back( fieldname );
                m_vocabs.push_back( voc );
                if( dname.size() ) {
                    m_types.push_back( IFT_dual1 );
                    fieldname = dname;
                    if( m_owner ) {
                        fieldname = m_owner->get_field_alias( dname );
                        dname = m_owner->get_num_code_alias( dname );
                    }
                    fieldout += "/" + dname;
                    m_output_fields.push_back( fieldname );
                    m_vocabs.push_back( NULL );
                    m_types.push_back( IFT_dual2 );
                } else {
                    m_types.push_back( type );
                }
                fname.clear();
                fieldname.clear();
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
                }
            }
            break;
        }
    }
    if( usefor != Use_input ) {
        m_output_str += fieldout;
    }
}

string Format::get_output_field( Vocab* vocab ) const
{
    for( size_t i = 0 ; i < m_vocabs.size() ; i++ ) {
        if( vocab == m_vocabs[i] ) {
            return m_output_fields[i];
        }
    }
    return "";
}

string Format::get_1st_output_field( InputFieldType type ) const
{
    for( size_t i = 0 ; i < m_types.size() ; i++ ) {
        if( type == m_types[i] ) {
            return m_output_fields[i];
        }
    }
    return "";
}

// End of src/cal/calformat.cpp file
